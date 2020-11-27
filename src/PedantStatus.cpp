#include "Definitions.h"
#include <Arduino.h>

/***************************
 *       Pedant Status 
 * *************************/

PedantStatus::PedantStatus(AxisSelection axis, xyScale x, xyScale y, zScale z) {
  selectedAxis = axis;
  scaleX = x;
  scaleY = y;
  scaleZ = z;
}
void PedantStatus::setSelectedAxis(AxisSelection axis) {
  selectedAxis = axis;

}
String PedantStatus::getSelectedStringScale(){
    switch (getSelectedAxis()) {
        case X:
          return getXScaleString();;
        case Y:
          return getYScaleString();;
        case Z:
          return getZScaleString();
        case UKNOWN:
          return getUknownScaleString();
        default:
          return getUknownScaleString();
    }
}

void PedantStatus::updateSelectedAxis(PedantButtonsState buttonsState){
    if (buttonsState.buttonXisPressed ){
        setSelectedAxis(X);
    } else if (buttonsState.buttonXisPressed ) {
      setSelectedAxis(Y);

    } else if (buttonsState.buttonXisPressed ) {
      setSelectedAxis(Z);
    }
    else if (buttonsState.buttonSCisPressed ){
    switch (getSelectedAxis()) {
        case X:
          modifyXScale(PedantStatus::INCREASE_OPP);
          break;
        case Y:
          modifyYScale(PedantStatus::INCREASE_OPP);
          break;
        case Z:
          modifyZScale(PedantStatus::INCREASE_OPP);
          break;
        default:
          break;
        }
    }
}

String PedantStatus::getSelectedAxisString() {
  switch (selectedAxis) {
    case X:
      return "X";
      break;
    case Y:
      return "Y";
      break;
    case Z:
      return "Z";
      break;
    case UKNOWN:
      return "0";
      break;
    default:
      return "0";
  }
}

xyScale PedantStatus::nextXYScaleValue(xyScale value) {
  switch (value) {
    case xy0_1MM: // 0,1 mm
      return xy0_5MM;
      break;
    case xy0_5MM: // 0,5 mm
      return xy1MM;
      break;
    case xy1MM: //  1 mm
      return xy10MM;
      break;
    case xy10MM: //  10mm
      return xy50MM;
      break;
    case xy50MM: //  50 mm
      return xy100MM;
      break;
    case xy100MM: //  50 mm
      return xy0_1MM;
      break;
    default:
      return xy0_1MM;
      break;
  }
}
xyScale PedantStatus::previousXYScaleValue(xyScale value) {
  switch (value) {
    case xy0_1MM: // 0,1 mm
      return xy100MM;
      break;
    case xy0_5MM: // 0,5 mm
      return xy0_1MM;
      break;
    case xy1MM: //  1 mm
      return xy0_5MM;
      break;
    case xy10MM: //  10mm
      return xy1MM;
      break;
    case xy50MM: //  50 mm
      return xy10MM;
      break;
    case xy100MM: //  50 mm
      return xy50MM;
      break;
    default:
      return xy0_1MM;
      break;
  }
}

void PedantStatus::modifyXScale(int operation) {
  if (operation == INCREASE_OPP) {
    scaleX = nextXYScaleValue(scaleX);
  }
  if (operation == DECREASE_OPP) {
    scaleX = previousXYScaleValue(scaleX);
  }
}
void PedantStatus::modifyYScale(int operation) {
  if (operation == INCREASE_OPP) {
    scaleY = nextXYScaleValue(scaleY);
  }
  if (operation == DECREASE_OPP) {
    scaleY = previousXYScaleValue(scaleY);
  }
}

zScale PedantStatus::nextZScaleValue(zScale value) {
  switch (value) {
  case z0_1MM: // 0,1 mm
    return z0_5MM;
    break;
  case z0_5MM: // 0,5 mm
    return z1MM;
    break;
  case z1MM: //  1 mm
    return z10MM;
    break;
  case z10MM: //  10mm
    return z50MM;
    break;
  case z50MM: //  50 mm
    return z0_1MM;
    break;
  default:
    return z0_1MM;
    break;
  }
}

zScale PedantStatus::previousZScaleValue(zScale value) {
  switch (value) {
  case z0_1MM: // 0,1 mm
    return z50MM;
    break;
  case z0_5MM: // 0,5 mm
    return z0_1MM;
    break;
  case z1MM: //  1 mm
    return z0_5MM;
    break;
  case z10MM: //  10mm
    return z1MM;
    break;
  case z50MM: //  50 mm
    return z10MM;
    break;
  default:
    return z0_1MM;
    break;
  }
}

void PedantStatus::modifyZScale(int operation) {
  if (operation == INCREASE_OPP) {
    scaleZ = nextZScaleValue(scaleZ);
  }
  if (operation == DECREASE_OPP) {
    scaleZ = previousZScaleValue(scaleZ);
  }
}

AxisSelection PedantStatus::getSelectedAxis() {
  return selectedAxis;

}
String PedantStatus::getXScaleString() {
  return xyScaleToString(scaleX);

}
String PedantStatus::getYScaleString() {
  return xyScaleToString(scaleY);
}
String PedantStatus::getZScaleString() {
  return zScaleToString(scaleZ);

}

String PedantStatus::getUknownScaleString() {
  return "0";

}

String PedantStatus::zScaleToString(zScale scale) {
  switch (scale) {
  case 0: // 0,1 mm
    return "0.1";
    break;
  case 1: // 0,5 mm
    return "0.5";
    break;
  case 2: //  1 mm
    return "1.0";
    break;
  case 3: //  10mm
    return " 10";
    break;
  case 4: //  50 mm
    return " 50";
    break;
  default:
    return "0.1";
    break;
  }
}

String PedantStatus::xyScaleToString(xyScale scale) {
  switch (scale) {
    case 0: // 0,1 mm
      return "0.1";
      break;
    case 1: // 0,5 mm
      return "0.5";
      break;
    case 2: //  1 mm
      return "1.0";
      break;
    case 3: //  10mm
      return " 10";
      break;
    case 4: //  50 mm
      return " 50";
      break;
    case 5: //  100 mm
      return "100";
      break;
    default:
      return "0.1";
      break;
  }
}