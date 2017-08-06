#include "navigator.h"

void InitNavigation()
{
  
}

void UpdateNavigation()
{
// =============================================================================
// Navigation Status Switching Algorithm:

  /* Heading part */
  if (marker_flag||lsm_flag) {
    to_fc.navigation_status |= HeadingOK;
  } else {
    to_fc.navigation_status &= ~HeadingOK;
  }

  /* Position part */
  if (marker_flag||gps_pos_flag) {
    to_fc.navigation_status |= PositionOK;
  } else {
    to_fc.navigation_status &= ~PositionOK;
  }

  /* Velocity part */
  if (marker_flag||gps_vel_flag) {
    to_fc.navigation_status |= VelocityOK;
  } else {
    to_fc.navigation_status &= ~VelocityOK;
  }

  /* low precision vertical part */
  // when marker is unavailable
  // altitude control is peformed by barometer
  if (!marker_flag) {
    to_fc.navigation_status |= LOW_PRECISION_VERTICAL;
  } else {
    to_fc.navigation_status &= ~LOW_PRECISION_VERTICAL;
  }

// =============================================================================
// Navigation mode Switching Algorithm:

  if (from_fc.nav_mode_request != nav_mode_) {
    switch (from_fc.nav_mode_request) {
      case NAV_MODE_AUTO:
      {
        if ((to_fc.navigation_status&HeadingOK)&&
        (to_fc.navigation_status&PositionOK)&&
        (to_fc.navigation_status&VelocityOK)) {
          nav_mode_ = NAV_MODE_AUTO;
        }
        break;
      }
      case NAV_MODE_HOLD:
      {
        if ((to_fc.navigation_status&HeadingOK)&&
        (to_fc.navigation_status&PositionOK)&&
        (to_fc.navigation_status&VelocityOK)) {
          nav_mode_ = NAV_MODE_HOLD;
          for (int i = 0; i < 3; i++) {
            hold_position[i] = to_fc.position[i];
          }
        }
        break;
      }
      case NAV_MODE_HOME:
      {
        // TO DO: Consider GO HOME algorithm
        nav_mode_ = NAV_MODE_HOME;
        break;
      }
      default:
      {
        nav_mode_ = NAV_MODE_OFF;
        break;
      }

    }
  }
  to_fc.nav_mode = nav_mode_;

// =============================================================================
// Target generation:

  switch (nav_mode_) {
    case NAV_MODE_AUTO:
    {
      break;
    }
    case NAV_MODE_HOLD:
    {
      for (int i = 0; i < 3; i++) {
        to_fc.target_position[i] = hold_position[i];
      }
      to_fc.transit_vel = DEFAULT_TRANSIT_SPEED;
      break;
    }
    case NAV_MODE_HOME:
    {
      break;
    }
    default:
    {
      break;
    }

  }    
}


void UpdateMarkerFlag()
{
  static uint8_t marker_notdetected_count = 0;
  if (from_marker.status) {
    marker_notdetected_count = 0;
    marker_flag = 1;
  } else {
    marker_notdetected_count += 1;
    if (marker_notdetected_count > 4) {
      marker_flag = 0;
      marker_notdetected_count = 5;
    }		
  }
}

void UpdateGPSPosFlag()
{
  if (from_gps.status&0x01) {
    gps_pos_flag = 1;
  } else {
    gps_pos_flag = 0;
  }
}

void UpdateGPSVelFlag()
{
  if (from_gps.status&0x02) {
    gps_vel_flag = 1;
  } else {
    gps_vel_flag = 0;
  }
}

void UpdateLSMFlag()
{
  if (from_lsm.status) {
    lsm_flag = 1;
  } else {
    lsm_flag = 0;
  }
}
