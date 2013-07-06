 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include "../include/Configuration.h"

//! Version
string Configuration::Version = "1.0.4.0";

//! Verbosity
unsigned int Configuration::Verbosity = 0;

//! Number of items in ignore list
int Configuration::IgnoreCount = 0;

//! List of uid to ignore
int Configuration::Ignore[800];

//! Value that specifies when processes needs to be killed
unsigned int Configuration::SoftSystemLimitMB = 0;

//! Value that specifies when processes needs to be killed
unsigned int Configuration::HardSystemLimitMB = 0;

//! Path to program to launch when process is killed
string Configuration::ExecPath = "";

//! Start a program after process is killed
bool Configuration::Exec = false;

//! Path to program to launch before process is killed
string Configuration::KillEP = "";

//! Execute this before kill of process
bool Configuration::KillExec = false;

//! Protect against kernel OOM killer
bool Configuration::ProtectSelf = true;

//! Whether processes should be killed when they exceed certain memory limit
bool Configuration::KillOOM = false;

//! Whether processes should be reported when they exceed certain memory limit
bool Configuration::ReportOOM = true;

//! Kill itself when best candidate
bool Configuration::KillSelf = false;

//! Whether swap should be considered as free memory too
bool Configuration::Swap = true;

//! If root processes can be killed too
bool Configuration::KillRoot = false;

//! Log
string Configuration::LF = "/var/log/terminator.log";

//! Type of log 0 - syslog 1 - stdout 2 - file
int Configuration::Logger = 1;

//! Pid of current process
pid_t Configuration::pid = getpid();

//! Limit for processes to kill with sigterm
unsigned int Configuration::HardMemoryLimitMB = 640;

//! Limit for processes to kill
unsigned int Configuration::SoftMemoryLimitMB = 620;

//! Memory of machine
unsigned long Configuration::MemoryTotal = 0;

//! Dry mode
bool Configuration::DryMode = false;

//! Interval between checks
int Configuration::Interval = 600000;

//! Running as background process
bool Configuration::Daemon = false;

