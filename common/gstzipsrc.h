#ifndef __GST_ZIPSRC_H__
#define __GST_ZIPSRC_H__

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>

class QuaZipFile;

G_BEGIN_DECLS

#define GST_TYPE_ZIPSRC \
  (gst_zip_src_get_type())
#define GST_ZIPSRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_ZIPSRC,GstZipSrc))
#define GST_ZIPSRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_ZIPSRC,GstZipSrcClass))
#define GST_IS_ZIPSRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_ZIPSRC))
#define GST_IS_ZIPSRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_ZIPSRC))

typedef struct _GstZipSrc      GstZipSrc;
typedef struct _GstZipSrcClass GstZipSrcClass;

struct _GstZipSrc
{
  GstBaseSrc element;

  guint64 read_position;
  gchar *uri;
  QuaZipFile *file;
};

struct _GstZipSrcClass
{
  GstBaseSrcClass parent_class;
};

GType gst_zip_src_get_type (void);

gboolean gst_zip_src_register();

G_END_DECLS

#endif /* __GST_ZIPSRC_H__ */
