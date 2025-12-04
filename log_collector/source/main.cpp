// ============================================================================
// File: main.cpp
// Description: Entry point for the TemplateApp MQTT application
// Author: Anurag
// Created: 2025-04-10
// Updated:
// License: Internal use only
// ============================================================================

#include "logcollector_application.hpp"

// Main function:
int main() {
  LogCollector app(APP_NAME); // Initialize with client ID

  app.init();
  app.run();

  return 0;
}
