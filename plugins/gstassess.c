/* GstSwitch
 * Copyright (C) 2013 Duzy Chan <code@duzy.info>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstassess.h"
#include "../logutils.h"

GST_DEBUG_CATEGORY_STATIC (gst_assess_debug);
#define GST_CAT_DEFAULT gst_assess_debug

#define GST_ASSESS_LOCK(obj) (g_mutex_lock (&(obj)->lock))
#define GST_ASSESS_UNLOCK(obj) (g_mutex_unlock (&(obj)->lock))

static GstStaticPadTemplate gst_assess_sink_factory =
  GST_STATIC_PAD_TEMPLATE ("sink",
      GST_PAD_SINK,
      GST_PAD_REQUEST,
      GST_STATIC_CAPS_ANY);

static GstStaticPadTemplate gst_assess_src_factory =
  GST_STATIC_PAD_TEMPLATE ("src",
      GST_PAD_SRC,
      GST_PAD_REQUEST,
      GST_STATIC_CAPS_ANY);

enum
{
  PROP_0,
};

#define gst_assess_parent_class parent_class
G_DEFINE_TYPE (GstAssess, gst_assess, GST_TYPE_BASE_TRANSFORM);

static void 
gst_assess_init (GstAssess *assess)
{
  g_mutex_init (&assess->lock);
}

static void
gst_assess_finalize (GstAssess *assess)
{
  g_mutex_clear (&assess->lock);

  G_OBJECT_CLASS (parent_class)->finalize (G_OBJECT (assess));
}

static void
gst_assess_set_property (GstAssess *assess, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  switch (prop_id) {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (G_OBJECT (assess), prop_id, pspec);
    break;
  }
}

static void
gst_assess_get_property (GstAssess *assess, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  switch (prop_id) {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (G_OBJECT (assess), prop_id, pspec);
    break;
  }
}

static gboolean
gst_assess_start (GstBaseTransform * trans)
{
  GstAssess *this = GST_ASSESS (trans);
  (void) this;
  return TRUE;
}

static gboolean
gst_assess_stop (GstBaseTransform * trans)
{
  GstAssess *this = GST_ASSESS (trans);
  (void) this;
  return TRUE;
}

static GstFlowReturn
gst_assess_transform (GstBaseTransform *trans, GstBuffer *buffer)
{
  GstFlowReturn ret;
  GstAssess *this = GST_ASSESS (trans);

  INFO ("buffer: %s, %p, %lld, %lld, %lld, %lld",
      GST_ELEMENT_NAME (trans), gst_element_get_parent (trans),
      GST_BUFFER_TIMESTAMP (buffer), GST_BUFFER_DURATION (buffer),
      GST_BUFFER_OFFSET (buffer), GST_BUFFER_OFFSET_END (buffer));

  (void) this;
  ret = GST_FLOW_OK;

  return ret;
}

static gboolean
gst_assess_sink_event (GstBaseTransform *trans, GstEvent *event)
{
  gboolean ret = TRUE;
  GstAssess *this = GST_ASSESS (trans);
  (void) this;
  ret = GST_BASE_TRANSFORM_CLASS (parent_class)->sink_event (trans, event);
  return ret;
}

static GstStateChangeReturn
gst_assess_change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret;
  GstAssess *this = GST_ASSESS (element);

  (void) this;

  switch (transition) {
  case GST_STATE_CHANGE_READY_TO_PAUSED:
    break;
  default:
    break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  switch (transition) {
  case GST_STATE_CHANGE_PAUSED_TO_READY:
    //gst_assess_reset (this);
    break;
  default:
    break;
  }

  return ret;
}

static void
gst_assess_class_init (GstAssessClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstBaseTransformClass *basetrans_class = GST_BASE_TRANSFORM_CLASS (klass);

  object_class->set_property = (GObjectSetPropertyFunc) gst_assess_set_property;
  object_class->get_property = (GObjectGetPropertyFunc) gst_assess_get_property;
  object_class->finalize = (GObjectFinalizeFunc) gst_assess_finalize;

  gst_element_class_set_static_metadata (element_class,
      "Stream Assessment", "Element",
      "Assess streams for frame drop and latency",
      "Duzy Chan <code@duzy.info>");

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_assess_sink_factory));

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_assess_src_factory));

  //element_class->request_new_pad = gst_assess_request_new_pad;
  //element_class->release_pad = gst_assess_release_pad;
  element_class->change_state = GST_DEBUG_FUNCPTR (gst_assess_change_state);

  basetrans_class->sink_event = GST_DEBUG_FUNCPTR (gst_assess_sink_event);
  basetrans_class->transform_ip = GST_DEBUG_FUNCPTR (gst_assess_transform);
  basetrans_class->start = GST_DEBUG_FUNCPTR (gst_assess_start);
  basetrans_class->stop = GST_DEBUG_FUNCPTR (gst_assess_stop);

  GST_DEBUG_CATEGORY_INIT (gst_assess_debug, "assess", 0, "Assess");
}
