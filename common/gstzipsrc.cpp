#include "gstzipsrc.h"
#include <QStringList>
#include <QDebug>
#include "quazipfile.h"

GST_DEBUG_CATEGORY_STATIC (gst_zip_src_debug);
#define GST_CAT_DEFAULT gst_zip_src_debug

#ifndef PACKAGE
#define PACKAGE "zipsrc"
#endif

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static void gst_zip_src_uri_handler_init (gpointer g_iface, gpointer iface_data);
static void gst_zip_src_finalize (GObject * object);
static gboolean gst_zip_src_start (GstBaseSrc * basesrc);
static gboolean gst_zip_src_stop (GstBaseSrc * basesrc);
static GstFlowReturn gst_zip_src_create (GstBaseSrc * basesrc, guint64 offset,
					 guint length, GstBuffer ** buffer);

static void gst_zip_src_do_init(GType type);

GST_BOILERPLATE_FULL (GstZipSrc, gst_zip_src, GstBaseSrc,
		      GST_TYPE_BASE_SRC, gst_zip_src_do_init);

static gboolean gst_zip_src_is_seekable(GstBaseSrc *src) {
  Q_UNUSED(src);

  return FALSE;
}

static GstURIType
gst_zip_src_uri_get_type () {
  return GST_URI_SRC;
}

static gchar **
gst_zip_src_uri_get_protocols () {
  static gchar *protocols[] = { (char *) "zip", NULL };

  return protocols;
}

static const gchar *
gst_zip_src_uri_get_uri (GstURIHandler * handler) {
  GstZipSrc *src = GST_ZIPSRC(handler);

  return src->uri;
}

static gboolean
gst_zip_src_uri_set_uri (GstURIHandler * handler, const gchar * uri) {
  GstZipSrc *src = GST_ZIPSRC(handler);

  GST_OBJECT_LOCK(src);

  GstState state = GST_STATE(src);

  GST_OBJECT_UNLOCK(src);

  if (state != GST_STATE_NULL && state != GST_STATE_READY) {
    return FALSE;
  }

  QString str = QString::fromUtf8(uri);
  if (str.isEmpty() || !str.startsWith("zip:", Qt::CaseInsensitive)) {
    return FALSE;
  }

  QStringList parts = str.split("//");
  if (parts.size() != 2) {
    return FALSE;
  }

  if (parts[0].size() <= 4) {
    return FALSE;
  }

  g_free(src->uri);
  src->uri = g_strdup(uri);

  return TRUE;
}

static void
gst_zip_src_do_init(GType type) {
  static const GInterfaceInfo urihandler_info = {
    gst_zip_src_uri_handler_init,
    NULL,
    NULL };

  g_type_add_interface_static (type, GST_TYPE_URI_HANDLER,
			       &urihandler_info);
}

static void
gst_zip_src_uri_handler_init (gpointer g_iface, gpointer iface_data) {
  Q_UNUSED(iface_data);

  GstURIHandlerInterface *iface = (GstURIHandlerInterface *) g_iface;

  iface->get_type = gst_zip_src_uri_get_type;
  iface->get_protocols = gst_zip_src_uri_get_protocols;
  iface->get_uri = gst_zip_src_uri_get_uri;
  iface->set_uri = gst_zip_src_uri_set_uri;
}

static void
gst_zip_src_base_init (gpointer gclass) {
  GstElementClass *element_class = GST_ELEMENT_CLASS (gclass);

  gst_element_class_set_details_simple(element_class,
    "ZipSrc",
    "Zip handler",
    "Source",
    "Mohammed Sameer <msameer@foolab.org>>");

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&src_factory));
}

/* initialize the zipsrc's class */
static void
gst_zip_src_class_init (GstZipSrcClass * klass) {
  GObjectClass *gobject_class;
  GstBaseSrcClass *gstbasesrc_class;

  gobject_class = (GObjectClass *) klass;
  gstbasesrc_class = (GstBaseSrcClass *)klass;

  gobject_class->finalize = gst_zip_src_finalize;
  gstbasesrc_class->is_seekable = GST_DEBUG_FUNCPTR(gst_zip_src_is_seekable);
  gstbasesrc_class->start = GST_DEBUG_FUNCPTR(gst_zip_src_start);
  gstbasesrc_class->stop = GST_DEBUG_FUNCPTR(gst_zip_src_stop);
  gstbasesrc_class->create = GST_DEBUG_FUNCPTR(gst_zip_src_create);
}

static void
gst_zip_src_init (GstZipSrc * src, GstZipSrcClass * gclass) {
  Q_UNUSED(gclass);

  src->file = NULL;
  src->uri = NULL;
}

static gboolean
zipsrc_init (GstPlugin * zipsrc) {
  GST_DEBUG_CATEGORY_INIT (gst_zip_src_debug, "zipsrc",
      0, "Zip handler");

  return gst_element_register (zipsrc, "zipsrc", GST_RANK_NONE,
			       GST_TYPE_ZIPSRC);
}

static void
gst_zip_src_finalize (GObject * object) {
  GstZipSrc *src = GST_ZIPSRC(object);

  delete src->file;
  src->file = NULL;

  g_free(src->uri);
  src->uri = NULL;

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_zip_src_start (GstBaseSrc * basesrc) {
  GstZipSrc *src = GST_ZIPSRC(basesrc);

  if (!src->uri || src->uri[0] == '\0') {
    GST_ELEMENT_ERROR (src, RESOURCE, NOT_FOUND, (NULL), (NULL));
    return FALSE;
  }

  delete src->file;

  QStringList parts = QString(src->uri).split("//");

  src->file = new QuaZipFile(parts[0].mid(4), parts[1]);

  if (!src->file->open(QIODevice::ReadOnly)) {
    GST_ELEMENT_ERROR (src, RESOURCE, READ, (NULL), (NULL));
    return FALSE;
  }

  src->read_position = 0;

  return TRUE;
}

static gboolean
gst_zip_src_stop (GstBaseSrc * basesrc) {
  GstZipSrc *src = GST_ZIPSRC(basesrc);

  delete src->file;

  src->file = NULL;

  return TRUE;
}

static GstFlowReturn gst_zip_src_create (GstBaseSrc * basesrc, guint64 offset,
					 guint length, GstBuffer ** buffer) {
  GstZipSrc *src = GST_ZIPSRC(basesrc);

  if (src->read_position != offset) {
    if (!src->file->seek(offset)) {
      return GST_FLOW_ERROR;
    }
  }

  GstBuffer *buf = gst_buffer_new_and_alloc(length);

  qint64 read = src->file->read((char *)GST_BUFFER_DATA(buf), length);

  if (read == -1) {
    gst_buffer_unref(buf);
    return GST_FLOW_ERROR;
  }
  else if (read == 0) {
    gst_buffer_unref(buf);
    return GST_FLOW_UNEXPECTED;
  }

  *buffer = buf;

  GST_BUFFER_SIZE(buf) = read;

  GST_BUFFER_OFFSET(buf) = offset;
  GST_BUFFER_OFFSET_END(buf) = offset + read;

  src->read_position += read;

  return GST_FLOW_OK;
}

gboolean
gst_zip_src_register() {
  return gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
				    "zipsrc", "Zip source",
				    zipsrc_init, VERSION,
				    "GPL", "", "", "");
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "zipsrc",
    "Zip source",
    zipsrc_init,
    VERSION,
    "GPL",
    "",
    ""
)
