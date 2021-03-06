#pragma once

class QDoubleValidator;
class QComboBox;
class QLineEdit;

typedef QLineEdit VDoubleInput;
typedef QLineEdit VIntegerInput;

// Needs to allow changing contents without having to worry about blocking signals
typedef QComboBox VDropdown;
