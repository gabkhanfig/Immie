// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* If this macro is defined, development specific functionality will be enabled. */
// UE_BUILD_SHIPPING
#define DEVELOPMENT

#ifdef DEVELOPMENT

/* Logger messages will be displayed on client / standalone user screen. */
#define DISPLAY_LOG_MESSAGES true

#else

/* Logger messages will not be displayed on screen. */
#define DISPLAY_LOG_MESSAGES false

#endif
