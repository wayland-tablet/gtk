#include "config.h"

#include "gdkattachparamsprivate.h"
#include "gdkscreen.h"
#include "gdkwindow.h"

/**
 * SECTION: gdkattachparams
 * @section_id: gdkattachparams
 * @title: Attachment Parameters
 * @short_description: Describing relative window position
 * @stability: Unstable
 * @include: gdk/gdkattachparams.h
 *
 * A full description of how a window should be positioned relative to an
 * attachment rectangle.
 *
 * Since: 3.20
 */

/**
 * gdk_attach_params_new:
 *
 * Creates a new #GdkAttachParams for describing the position of a #GdkWindow
 * relative to an attachment #GdkRectangle.
 *
 * Returns: (transfer full): a new #GdkAttachParams, to be freed with
 *          gdk_attach_params_free()
 *
 * Since: 3.20
 */
GdkAttachParams *
gdk_attach_params_new (void)
{
  GdkAttachParams *params = g_new0 (GdkAttachParams, 1);

  params->primary_rules = g_array_new (TRUE, TRUE, sizeof (GdkAttachRule));
  params->secondary_rules = g_array_new (TRUE, TRUE, sizeof (GdkAttachRule));

  return params;
}

/**
 * gdk_attach_params_copy:
 * @src: the #GdkAttachParams to copy
 * @data: (nullable): unused
 *
 * Creates a deep copy of @src.
 *
 * Returns: (transfer full) (nullable): a deep copy of @src, to be freed with
 *          gdk_attach_params_free()
 *
 * Since: 3.20
 */
gpointer
gdk_attach_params_copy (gconstpointer src,
                        gpointer      data)
{
  GdkAttachParams *copy;
  const GdkAttachParams *params;

  if (!src)
    return NULL;

  params = src;

  copy = g_memdup (params, sizeof (*params));

  copy->primary_rules = g_array_sized_new (TRUE, TRUE, sizeof (GdkAttachRule),
                                           params->primary_rules->len);

  g_array_append_vals (copy->primary_rules,
                       params->primary_rules->data,
                       params->primary_rules->len);

  copy->secondary_rules = g_array_sized_new (TRUE, TRUE, sizeof (GdkAttachRule),
                                             params->secondary_rules->len);

  g_array_append_vals (copy->secondary_rules,
                       params->secondary_rules->data,
                       params->secondary_rules->len);

  return copy;
}

/**
 * gdk_attach_params_free:
 * @data: the #GdkAttachParams to free
 *
 * Releases @data.
 *
 * Since: 3.20
 */
void
gdk_attach_params_free (gpointer data)
{
  GdkAttachParams *params;

  g_return_if_fail (data);

  params = data;

  if (params->attach_user_data && params->attach_destroy_notify)
    params->attach_destroy_notify (params->attach_user_data);

  g_array_unref (params->secondary_rules);
  g_array_unref (params->primary_rules);

  g_free (params);
}

/**
 * gdk_attach_params_set_attach_origin:
 * @params: a #GdkAttachParams
 * @origin: (nullable): the attachment rectangle's origin
 *
 * Sets the origin of the attachment rectangle's coordinate system in root
 * coordinates.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_attach_origin (GdkAttachParams *params,
                                     const GdkPoint  *origin)
{
  GdkPoint zero = { 0 };

  g_return_if_fail (params);

  params->attach_origin = origin ? *origin : zero;
}

/**
 * gdk_attach_params_set_attach_rect:
 * @params: a #GdkAttachParams
 * @rectangle: (nullable): the attachment rectangle
 *
 * Sets the attachment rectangle the window needs to be aligned relative to.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_attach_rect (GdkAttachParams    *params,
                                   const GdkRectangle *rectangle)
{
  g_return_if_fail (params);

  if (rectangle)
    {
      params->has_attach_rect = TRUE;
      params->attach_rect = *rectangle;
    }
  else
    params->has_attach_rect = FALSE;
}

/**
 * gdk_attach_params_set_attach_margin:
 * @params: a #GdkAttachParams
 * @margin: (nullable): the space around the attachment rectangle
 *
 * Sets the amount of space to leave around the attachment rectangle.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_attach_margin (GdkAttachParams *params,
                                     const GdkBorder *margin)
{
  GdkBorder zero = { 0 };

  g_return_if_fail (params);

  params->attach_margin = margin ? *margin : zero;
}

/**
 * gdk_attach_params_set_window_margin:
 * @params: a #GdkAttachParams
 * @margin: (nullable): the space around the window
 *
 * Sets the amount of space to leave around the window.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_window_margin (GdkAttachParams *params,
                                     const GdkBorder *margin)
{
  GdkBorder zero = { 0 };

  g_return_if_fail (params);

  params->window_margin = margin ? *margin : zero;
}

/**
 * gdk_attach_params_set_window_padding:
 * @params: a #GdkAttachParams
 * @padding: (nullable): the space between the window and its
 *           contents.
 *
 * Sets the amount of space between the window and its contents.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_window_padding (GdkAttachParams *params,
                                      const GdkBorder *padding)
{
  GdkBorder zero = { 0 };

  g_return_if_fail (params);

  params->window_padding = padding ? *padding : zero;
}

/**
 * gdk_attach_params_set_window_offset:
 * @params: a #GdkAttachParams
 * @offset: (nullable): the window displacement
 *
 * Sets the offset to displace the window by.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_window_offset (GdkAttachParams *params,
                                     const GdkPoint  *offset)
{
  GdkPoint zero = { 0 };

  g_return_if_fail (params);

  params->window_offset = offset ? *offset : zero;
}

/**
 * gdk_attach_params_get_window_type_hint:
 * @params: a #GdkAttachParams
 *
 * Gets the window type hint set on @params.
 *
 * Returns: the window type hint set on @params
 *
 * Since: 3.20
 */
GdkWindowTypeHint
gdk_attach_params_get_window_type_hint (const GdkAttachParams *params)
{
  g_return_val_if_fail (params, GDK_WINDOW_TYPE_HINT_NORMAL);

  return params->window_type_hint;
}

/**
 * gdk_attach_params_set_window_type_hint:
 * @params: a #GdkAttachParams
 * @window_type_hint: the window type hint
 *
 * Sets the window type hint.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_window_type_hint (GdkAttachParams   *params,
                                        GdkWindowTypeHint  window_type_hint)
{
  g_return_if_fail (params);

  params->window_type_hint = window_type_hint;
}

/**
 * gdk_attach_params_set_right_to_left:
 * @params: a #GdkAttachParams
 * @is_right_to_left: %TRUE if the text direction is right to left
 *
 * Sets the window text direction.
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_right_to_left (GdkAttachParams *params,
                                     gboolean         is_right_to_left)
{
  g_return_if_fail (params);

  params->is_right_to_left = is_right_to_left;
}

static void
add_rules (GArray        *array,
           GdkAttachRule  first_rule,
           va_list        args)
{
  GdkAttachRule rule;

  for (rule = first_rule; rule; rule = va_arg (args, GdkAttachRule))
    g_array_append_val (array, rule);
}

/**
 * gdk_attach_params_add_primary_rules_valist:
 * @params: a #GdkAttachParams
 * @first_rule: first primary rule
 * @args: a #va_list of the remaining primary rules
 *
 * Non-variadic version of gdk_attach_params_add_primary_rules().
 *
 * Since: 3.20
 */
void
gdk_attach_params_add_primary_rules_valist (GdkAttachParams *params,
                                            GdkAttachRule    first_rule,
                                            va_list          args)
{
  add_rules (params->primary_rules, first_rule, args);
}

/**
 * gdk_attach_params_add_secondary_rules_valist:
 * @params: a #GdkAttachParams
 * @first_rule: first secondary rule
 * @args: a #va_list of the remaining secondary rules
 *
 * Non-variadic version of gdk_attach_params_add_secondary_rules().
 *
 * Since: 3.20
 */
void
gdk_attach_params_add_secondary_rules_valist (GdkAttachParams *params,
                                              GdkAttachRule    first_rule,
                                              va_list          args)
{
  add_rules (params->secondary_rules, first_rule, args);
}

/**
 * gdk_attach_params_add_primary_rules:
 * @params: a #GdkAttachParams
 * @first_rule: first primary rule
 * @...: a %NULL-terminated list of rules
 *
 * Appends to the list of primary positioning rules to try.
 *
 * A typical backend will try each primary rule in the order they're added. If
 * a rule can be satisfied, it will then try each secondary rule until it
 * finds a satisfiable secondary rule that doesn't conflict with the primary
 * rule. If it finds a pair of satisfiable non-conflicting rules, then it will
 * place the window there. If it cannot find a pair, it proceeds to the next
 * primary rule and tries again.
 *
 * Since: 3.20
 */
void
gdk_attach_params_add_primary_rules (GdkAttachParams *params,
                                     GdkAttachRule    first_rule,
                                     ...)
{
  va_list args;

  g_return_if_fail (params);

  va_start (args, first_rule);

  gdk_attach_params_add_primary_rules_valist (params, first_rule, args);

  va_end (args);
}

/**
 * gdk_attach_params_add_secondary_rules:
 * @params: a #GdkAttachParams
 * @first_rule: first secondary rule
 * @...: a %NULL-terminated list of rules
 *
 * Appends to the list of secondary positioning rules to try.
 *
 * A typical backend will try each primary rule in the order they're added. If
 * a rule can be satisfied, it will then try each secondary rule until it
 * finds a satisfiable secondary rule that doesn't conflict with the primary
 * rule. If it finds a pair of satisfiable non-conflicting rules, then it will
 * place the window there. If it cannot find a pair, it proceeds to the next
 * primary rule and tries again.
 *
 * Since: 3.20
 */
void
gdk_attach_params_add_secondary_rules (GdkAttachParams *params,
                                       GdkAttachRule    first_rule,
                                       ...)
{
  va_list args;

  g_return_if_fail (params);

  va_start (args, first_rule);

  gdk_attach_params_add_secondary_rules_valist (params, first_rule, args);

  va_end (args);
}

/**
 * gdk_attach_params_set_position_callback:
 * @params: a #GdkAttachParams
 * @callback: (nullable): a function to be called when the final position of
 *            the window is known
 * @user_data: (transfer full) (nullable): additional data to pass to @callback
 * @destroy_notify: (nullable): a function to release @user_data
 *
 * Sets the function to be called when the final position of the window is
 * known. Since the position might be determined asynchronously, don't assume
 * it will be called directly from gdk_window_set_attach_params().
 *
 * Since: 3.20
 */
void
gdk_attach_params_set_position_callback (GdkAttachParams   *params,
                                         GdkAttachCallback  callback,
                                         gpointer           user_data,
                                         GDestroyNotify     destroy_notify)
{
  g_return_if_fail (params);

  params->attach_callback = callback;

  if (user_data != params->attach_user_data)
    {
      if (params->attach_user_data && params->attach_destroy_notify)
        params->attach_destroy_notify (params->attach_user_data);

      params->attach_user_data = user_data;
      params->attach_destroy_notify = destroy_notify;
    }
  else if (user_data)
    g_warning ("%s (): params already owns user data", G_STRFUNC);
}

static gboolean
is_satisfiable (GdkAttachRule          rule,
                const GdkAttachParams *params,
                gint                   width,
                gint                   height,
                const GdkRectangle    *bounds,
                gint                  *value)
{
  gboolean use_rect_margin;
  gboolean use_window_margin;
  gboolean use_window_padding;

  g_return_val_if_fail (params, FALSE);
  g_return_val_if_fail (params->has_attach_rect, FALSE);

  if (params->is_right_to_left && (rule & GDK_ATTACH_FLIP_IF_RTL))
    {
      switch (rule & GDK_ATTACH_RECT_MASK)
        {
        case GDK_ATTACH_RECT_MIN:
          rule = (rule & ~(GDK_ATTACH_RECT_MASK | GDK_ATTACH_FLIP_IF_RTL)) | GDK_ATTACH_RECT_MAX;
          break;

        case GDK_ATTACH_RECT_MAX:
          rule = (rule & ~(GDK_ATTACH_RECT_MASK | GDK_ATTACH_FLIP_IF_RTL)) | GDK_ATTACH_RECT_MIN;
          break;
        }

      switch (rule & GDK_ATTACH_WINDOW_MASK)
        {
        case GDK_ATTACH_WINDOW_MIN:
          rule = (rule & ~(GDK_ATTACH_WINDOW_MASK | GDK_ATTACH_FLIP_IF_RTL)) | GDK_ATTACH_WINDOW_MAX;
          break;

        case GDK_ATTACH_WINDOW_MAX:
          rule = (rule & ~(GDK_ATTACH_WINDOW_MASK | GDK_ATTACH_FLIP_IF_RTL)) | GDK_ATTACH_WINDOW_MIN;
          break;
        }
    }

  use_rect_margin = (((rule & GDK_ATTACH_RECT_MASK)   == GDK_ATTACH_RECT_MIN &&
                      (rule & GDK_ATTACH_WINDOW_MASK) == GDK_ATTACH_WINDOW_MAX) ||
                     ((rule & GDK_ATTACH_RECT_MASK)   == GDK_ATTACH_RECT_MAX &&
                      (rule & GDK_ATTACH_WINDOW_MASK) == GDK_ATTACH_WINDOW_MIN));

  use_window_margin = use_rect_margin;

  use_window_padding = TRUE;

  switch (rule & GDK_ATTACH_AXIS_MASK)
    {
    case GDK_ATTACH_AXIS_X:
      *value = params->attach_origin.x;

      switch (rule & GDK_ATTACH_RECT_MASK)
        {
        case GDK_ATTACH_RECT_MIN:
          *value += params->attach_rect.x;

          if (use_rect_margin)
            *value -= params->attach_margin.left;

          break;

        case GDK_ATTACH_RECT_MID:
          *value += params->attach_rect.x + params->attach_rect.width / 2;
          break;

        case GDK_ATTACH_RECT_MAX:
          *value += params->attach_rect.x + params->attach_rect.width;

          if (use_rect_margin)
            *value += params->attach_margin.right;

          break;
        }

      switch (rule & GDK_ATTACH_WINDOW_MASK)
        {
        case GDK_ATTACH_WINDOW_MIN:
          if (use_window_margin)
            *value += params->window_margin.left;

          if (use_window_padding)
            *value -= params->window_margin.left;

          break;

        case GDK_ATTACH_WINDOW_MID:
          *value -= width / 2;
          break;

        case GDK_ATTACH_WINDOW_MAX:
          *value -= width;

          if (use_window_margin)
            *value -= params->window_margin.right;

          if (use_window_padding)
            *value += params->window_margin.right;

          break;
        }

      *value += params->window_offset.x;

      return !bounds || (bounds->x <= *value && *value + width <= bounds->x + bounds->width);

    case GDK_ATTACH_AXIS_Y:
      *value = params->attach_origin.y;

      switch (rule & GDK_ATTACH_RECT_MASK)
        {
        case GDK_ATTACH_RECT_MIN:
          *value += params->attach_rect.y;

          if (use_rect_margin)
            *value -= params->attach_margin.top;

          break;

        case GDK_ATTACH_RECT_MID:
          *value += params->attach_rect.y + params->attach_rect.height / 2;
          break;

        case GDK_ATTACH_RECT_MAX:
          *value += params->attach_rect.y + params->attach_rect.height;

          if (use_rect_margin)
            *value += params->attach_margin.bottom;

          break;
        }

      switch (rule & GDK_ATTACH_WINDOW_MASK)
        {
        case GDK_ATTACH_WINDOW_MIN:
          if (use_window_margin)
            *value += params->window_margin.top;

          if (use_window_padding)
            *value -= params->window_margin.top;

          break;

        case GDK_ATTACH_WINDOW_MID:
          *value -= height / 2;
          break;

        case GDK_ATTACH_WINDOW_MAX:
          *value -= height;

          if (use_window_margin)
            *value -= params->window_margin.bottom;

          if (use_window_padding)
            *value += params->window_margin.bottom;

          break;
        }

      *value += params->window_offset.y;

      return !bounds || (bounds->y <= *value && *value + height <= bounds->y + bounds->height);
    }

  return FALSE;
}

#define BEST      0
#define GOOD      1
#define PRIMARY   0
#define SECONDARY 1
#define X         0
#define Y         1

/**
 * gdk_attach_params_choose_position:
 * @params: a #GdkAttachParams
 * @width: window width
 * @height: window height
 * @bounds: (nullable): monitor geometry
 * @position: (out) (optional): the best position for the window
 * @offset: (out) (optional): the displacement needed to push the window
 *          on-screen
 * @primary_rule: (out) (optional): the best primary rule
 * @secondary_rule: (out) (optional): the best secondary rule
 *
 * Finds the best position for a window of size @width and @height on a screen
 * with @bounds using the given @params.
 *
 * Returns: %TRUE if there is a pair of satisfiable primary and secondary
 *          rules that do not conflict with each other
 *
 * Since: 3.20
 */
gboolean
gdk_attach_params_choose_position (const GdkAttachParams *params,
                                   gint                   width,
                                   gint                   height,
                                   const GdkRectangle    *bounds,
                                   GdkPoint              *position,
                                   GdkPoint              *offset,
                                   GdkAttachRule         *primary_rule,
                                   GdkAttachRule         *secondary_rule)
{
  GdkPoint p;
  GdkPoint o;
  GdkAttachRule pr;
  GdkAttachRule sr;
  GdkAttachRule rules[2][2][2] = { 0 };
  gint axes[2][2] = { 0 };
  gint values[2][2][2] = { 0 };
  GArray *arrays[2];
  gint i;
  gint j;
  gint k;
  GdkAttachRule rule;
  gint axis;
  gint value;
  gboolean satisfiable;
  gboolean success;
  GdkAttachRule x_rule;
  GdkAttachRule y_rule;

  g_return_val_if_fail (params, FALSE);
  g_return_val_if_fail (params->has_attach_rect, FALSE);

  if (!position)
    position = &p;

  if (!offset)
    offset = &o;

  if (!primary_rule)
    primary_rule = &pr;

  if (!secondary_rule)
    secondary_rule = &sr;

  arrays[PRIMARY] = params->primary_rules;
  arrays[SECONDARY] = params->secondary_rules;

  for (i = PRIMARY; i <= SECONDARY; i++)
    {
      for (j = 0; j < arrays[i]->len; j++)
        {
          rule = g_array_index (arrays[i], GdkAttachRule, j);

          switch (rule & GDK_ATTACH_AXIS_MASK)
            {
            case GDK_ATTACH_AXIS_X:
              axis = X;
              break;

            case GDK_ATTACH_AXIS_Y:
              axis = Y;
              break;

            default:
              axis = -1;
              break;
            }

          if (axis < 0)
            {
              g_warning ("%s (): invalid constraint axis: 0x%X", G_STRFUNC, rule);
              continue;
            }

          satisfiable = is_satisfiable (rule, params, width, height, bounds, &value);

          if (satisfiable && !rules[i][BEST][axis])
            {
              rules[i][BEST][axis] = rule;
              values[i][BEST][axis] = value;

              if (rules[i][BEST][!axis])
                break;
              else
                axes[i][BEST] = axis;
            }
          else if (!rules[i][GOOD][axis])
            {
              rules[i][GOOD][axis] = rule;
              values[i][GOOD][axis] = value;

              if (!rules[i][GOOD][!axis])
                axes[i][GOOD] = axis;
            }
        }
    }

  success = FALSE;

  for (i = BEST; i <= GOOD; i++)
    {
      for (j = BEST; j <= GOOD; j++)
        {
          for (k = Y; k >= X; k--)
            {
              if (rules[PRIMARY][i][axes[PRIMARY][i] == k] && rules[SECONDARY][j][axes[PRIMARY][i] != k])
                {
                  *primary_rule = rules[PRIMARY][i][axes[PRIMARY][i] == k];
                  *secondary_rule = rules[SECONDARY][j][axes[PRIMARY][i] != k];

                  if ((axes[PRIMARY][i] == k) == X)
                    {
                      position->x = values[PRIMARY][i][X];
                      position->y = values[SECONDARY][j][Y];
                    }
                  else
                    {
                      position->x = values[SECONDARY][j][X];
                      position->y = values[PRIMARY][i][Y];
                    }

                  offset->x = 0;
                  offset->y = 0;
                  success = TRUE;
                  break;
                }
            }

          if (success)
            break;
        }

      if (success)
        break;
    }

  if (success && bounds)
    {
      if ((*primary_rule & GDK_ATTACH_AXIS_MASK) == GDK_ATTACH_AXIS_X)
        {
          x_rule = *primary_rule;
          y_rule = *secondary_rule;
        }
      else
        {
          x_rule = *secondary_rule;
          y_rule = *primary_rule;
        }

      if (params->is_right_to_left && (x_rule & GDK_ATTACH_FLIP_IF_RTL))
        {
          if (position->x < bounds->x)
            {
              offset->x += bounds->x - position->x;
              position->x = bounds->x;
            }

          if (position->x + width > bounds->x + bounds->width)
            {
              offset->x += bounds->x + bounds->width - width - position->x;
              position->x = bounds->x + bounds->width - width;
            }
        }
      else
        {
          if (position->x + width > bounds->x + bounds->width)
            {
              offset->x += bounds->x + bounds->width - width - position->x;
              position->x = bounds->x + bounds->width - width;
            }

          if (position->x < bounds->x)
            {
              offset->x += bounds->x - position->x;
              position->x = bounds->x;
            }
        }

      if (params->is_right_to_left && (y_rule & GDK_ATTACH_FLIP_IF_RTL))
        {
          if (position->y < bounds->y)
            {
              offset->y += bounds->y - position->y;
              position->y = bounds->y;
            }

          if (position->y + height > bounds->y + bounds->height)
            {
              offset->y += bounds->y + bounds->height - height - position->y;
              position->y = bounds->y + bounds->height - height;
            }
        }
      else
        {
          if (position->y + height > bounds->y + bounds->height)
            {
              offset->y += bounds->y + bounds->height - height - position->y;
              position->y = bounds->y + bounds->height - height;
            }

          if (position->y < bounds->y)
            {
              offset->y += bounds->y - position->y;
              position->y = bounds->y;
            }
        }
    }

  return success;
}

/**
 * gdk_attach_params_choose_position_for_window:
 * @params: a #GdkAttachParams
 * @window: (transfer none) (not nullable): the #GdkWindow to find the best
 *          position for
 * @position: (out) (optional): the best position for the window
 * @offset: (out) (optional): the displacement needed to push the window
 *          on-screen
 * @primary_rule: (out) (optional): the best primary rule
 * @secondary_rule: (out) (optional): the best secondary rule
 *
 * Finds the best position for @window according to @params.
 *
 * Returns: %TRUE if there's a best position
 *
 * Since: 3.20
 */
gboolean
gdk_attach_params_choose_position_for_window (const GdkAttachParams *params,
                                              GdkWindow             *window,
                                              GdkPoint              *position,
                                              GdkPoint              *offset,
                                              GdkAttachRule         *primary_rule,
                                              GdkAttachRule         *secondary_rule)
{
  GdkScreen *screen;
  gint x;
  gint y;
  gint monitor;
  GdkRectangle bounds;
  gint width;
  gint height;

  g_return_val_if_fail (params, FALSE);
  g_return_val_if_fail (params->has_attach_rect, FALSE);
  g_return_val_if_fail (window, FALSE);

  screen = gdk_window_get_screen (window);
  x = params->attach_origin.x + params->attach_rect.x + params->attach_rect.width / 2;
  y = params->attach_origin.y + params->attach_rect.y + params->attach_rect.height / 2;
  monitor = gdk_screen_get_monitor_at_point (screen, x, y);
  gdk_screen_get_monitor_workarea (screen, monitor, &bounds);
  width = gdk_window_get_width (window);
  height = gdk_window_get_height (window);

  return gdk_attach_params_choose_position (params,
                                            width,
                                            height,
                                            &bounds,
                                            position,
                                            offset,
                                            primary_rule,
                                            secondary_rule);
}

/**
 * gdk_window_move_using_attach_params:
 * @window: the #GdkWindow to position
 * @params: (nullable): a description of how to position @window
 *
 * Moves @window to the best position according to @params.
 *
 * Since: 3.20
 */
void
gdk_window_move_using_attach_params (GdkWindow             *window,
                                     const GdkAttachParams *params)
{
  GdkPoint position;
  GdkPoint offset;
  GdkAttachRule primary_rule;
  GdkAttachRule secondary_rule;

  g_return_if_fail (GDK_IS_WINDOW (window));

  if (!params || !params->has_attach_rect)
    return;

  if (gdk_attach_params_choose_position_for_window (params,
                                                    window,
                                                    &position,
                                                    &offset,
                                                    &primary_rule,
                                                    &secondary_rule))
    {
      gdk_window_move (window, position.x, position.y);

      if (params->attach_callback)
        params->attach_callback (window,
                                 params,
                                 &position,
                                 &offset,
                                 primary_rule,
                                 secondary_rule,
                                 params->attach_user_data);
    }
}
