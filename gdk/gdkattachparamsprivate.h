#ifndef __GDK_ATTACH_PARAMS_PRIVATE_H__
#define __GDK_ATTACH_PARAMS_PRIVATE_H__

#include "gdkattachparams.h"

G_BEGIN_DECLS

/*
 * GdkAttachParams:
 * @attach_origin: root origin of @attach_rect coordinate system
 * @has_attach_rect: %TRUE if @attach_rect is valid
 * @attach_rect: the attachment rectangle to attach the window to
 * @attach_margin: the space to leave around @attach_rect
 * @window_margin: the space to leave around the window
 * @window_padding: the space between the window and its contents
 * @window_offset: the offset to displace the window by
 * @window_type_hint: the window type hint
 * @is_right_to_left: %TRUE if the text direction is right to left
 * @primary_rules: an array of primary #GdkAttachRule
 * @secondary_rules: an array of secondary #GdkAttachRule
 * @position_callback: a function to call when the final position is known
 * @position_callback_user_data: additional data to pass to @position_callback
 * @position_callback_destroy_notify: a function to free
 *                                    @position_callback_user_data
 *
 * Opaque type containing the information needed to position a window relative
 * to an attachment rectangle.
 *
 * Since: 3.20
 */
struct _GdkAttachParams
{
  /*< private >*/
  GdkPoint attach_origin;

  gboolean has_attach_rect;
  GdkRectangle attach_rect;

  GdkBorder attach_margin;
  GdkBorder window_margin;
  GdkBorder window_padding;

  GdkPoint window_offset;

  GdkWindowTypeHint window_type_hint;

  gboolean is_right_to_left;

  GArray *primary_rules;
  GArray *secondary_rules;

  GdkAttachCallback attach_callback;
  gpointer attach_user_data;
  GDestroyNotify attach_destroy_notify;
};

G_GNUC_INTERNAL
gboolean gdk_attach_params_choose_position            (const GdkAttachParams *params,
                                                       gint                   width,
                                                       gint                   height,
                                                       const GdkRectangle    *bounds,
                                                       GdkPoint              *position,
                                                       GdkPoint              *offset,
                                                       GdkAttachRule         *primary_rule,
                                                       GdkAttachRule         *secondary_rule);

G_GNUC_INTERNAL
gboolean gdk_attach_params_choose_position_for_window (const GdkAttachParams *params,
                                                       GdkWindow             *window,
                                                       GdkPoint              *position,
                                                       GdkPoint              *offset,
                                                       GdkAttachRule         *primary_rule,
                                                       GdkAttachRule         *secondary_rule);

G_GNUC_INTERNAL
void     gdk_window_move_using_attach_params          (GdkWindow             *window,
                                                       const GdkAttachParams *params);

G_END_DECLS

#endif /* __GDK_ATTACH_PARAMS_PRIVATE_H__ */
