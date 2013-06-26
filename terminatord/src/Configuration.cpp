#include "../include/Configuration.h"

//! Version
string Configuration::Version = "1.0.0.0";

//! Verbosity
unsigned int Configuration::Verbosity = 0;

int Configuration::IgnoreCount = 0;

int Configuration::Ignore[800];

unsigned int Configuration::SoftSystemLimitMB = 0;

unsigned int Configuration::HardSystemLimitMB = 0;

//! Whether processes should be killed when they exceed certain memory limit
bool Configuration::KillOOM = false;

//! Whether processes should be reported when they exceed certain memory limit
bool Configuration::ReportOOM = true;

//! If root processes can be killed too
bool Configuration::KillRoot = false;

//! Limit for processes to kill with sigterm
unsigned int Configuration::HardMemoryLimitMB = 64;

//! Limit for processes to kill
unsigned int Configuration::SoftMemoryLimitMB = 32;

unsigned long Configuration::MemoryTotal = 0;

bool Configuration::DryMode = false;

//! Interval between checks
int Configuration::Interval = 600000;

bool Configuration::Daemon = false;

