/* js-mode */

function scrollToItem(iten) {
  var upper = flick.mapFromItem(item, 0, item.y).y + flick.contentY;
  var lower = flick.mapFromItem(item, 0, item.y + item.height).y + flick.contentY;

  // Let's reset here. We have multiple exit points.
  pagePosition.reset();

  if (upper >= flick.contentY && lower <= flick.contentY + flick.height) {
    // Nothing.
    return;
  }
  else if (lower <= flick.contentY + flick.height) {
    // Topmost part is not visible.
    // We will scroll anyway and make it visible.
    animation.run(upper);
    return;
  }

  if (lower - upper > flick.height) {
    // The line will not fit no matter what we do.
    // Just show the upper part.
    animation.run(upper);
    return;
  }

  // Our line will fit the view. We need to scroll until the bottommost part
  // is just visible.
  var part = upper + (lower - (upper + flick.height));

  animation.run(part);
}
