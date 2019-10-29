// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
// 
/// ==========================================================================
// Name    :    SetupCodes.h
// Purpose :    This file contains the errors that 
//              Setup and related tools return

#ifndef SetupCodes_h_
#define SetupCodes_h_

const int COR_REBOOT_REQUIRED           = 0x002000; // (8192) Reboot is required

const int COR_USAGE_ERROR               = 0x001000; // (4096) Improper usage/invalid parameters
const int COR_INSUFFICIENT_PRIVILEGES   = 0x001001; // (4097) On NT, Need Admin rights to (un)install
const int COR_DARWIN_INSTALL_FAILURE    = 0x001002; // (4098) Installation of Darwin components failed
const int COR_DARWIN_NOT_INSTALLED      = 0x001003; // (4099) Windows Installer is not installed properly on machine

const int COR_SINGLE_INSTANCE_FAIL      = 0x001004; // (4100) CreateMutex failed
const int COR_NOT_SINGLE_INSTANCE       = 0x001005; // (4101) Another instance of setup is already running
const int COR_CARRIER_NOT_EXIST         = 0x001006; // (4102) Carrier product did not install Framework on the system
const int COR_CARRIER_EXIST             = 0x001007; // (4103) Carrier product already installed Framework on the system

const int COR_FRAMEWORK_EXIST           = 0x001008; // (4104) Framework (PackageType, Version) pair is already installed
const int COR_FRAMEWORK_NOT_EXIST       = 0x001009; // (4105) Framework (PackageType, Version) pair does not exist

const int COR_CANNOT_GET_WINDOWS_DIR    = 0x00100A; // (4106) Cannot get Windows directory
const int COR_MSI_OPEN_ERROR            = 0x00100B; // (4107) Cannot open MSI Database
const int COR_MSI_READ_ERROR            = 0x00100C; // (4108) Cannot read from MSI Database
const int COR_REGISTRY_ERROR            = 0x00100D; // (4109) Cannot read/write to registry
const int COR_CANNOT_LOAD_OLEACC        = 0x00100E; // (4110) Cannot load oleacc.dll
const int COR_CANNOT_GET_TEMP_DIR       = 0x00100F; // (4111) Cannot get Temp directory
const int COR_CARRIERNAME_TOOLONG       = 0x001010; // (4112) In /p <Product>, more than 128 chars for <Product>
const int COR_OLD_FRAMEWORK_EXIST       = 0x001011; // (4113) beta NDP components detected
const int COR_CANNOT_GET_SYSTEM_DIR     = 0x001012; // (4114) Cannot get System directory

const int COR_EXIT_FAILURE              = 0x001FFF; // (8191) Setup Failure - unknown reason

#endif

