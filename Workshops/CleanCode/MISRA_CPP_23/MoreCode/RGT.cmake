#/************************************************************************
# Basis CMake script voor het CS practicum
# Copyright 2004-2020 ir drs E.J Boks
# Opleiding Embedded Systems Engineering
# Technical University
# $Id: CS.cmake 3475 2019-04-17 13:22:02Z dev $
#************************************************************************/


## BELANGRIJK ..... IMPORTANT ........
## Zet hier de gewenste taal :
## Select the preferred language :
set(InterfaceTaal Nederlands CACHE STRING "Kies voor het Nederlandstalige practicum")   # Voor Nederlands
#set(InterfaceTaal English CACHE STRING "Perform the practical work in English" )  # For English

## BELANGRIJK ..... IMPORTANT ........
## Zet hier het practicum dat wordt uitgevoerd.
# This is the Debug Assignment. Uncomment for debug session 1-2
#set(practicumopdracht DebugOpdracht CACHE STRING "De debugopdracht voorafgaand aan de ECS opdrachten")

# This is the Input Response assignment.  Uncomment for ECS assignement 1
set(practicumopdracht InputResponsieOpdracht CACHE STRING "Bouw CS inputresponsies")

# This is the PID  Controller assignment.  Uncomment for ECS assignement 2-5
#set(practicumopdracht PIDControllerOpdracht CACHE STRING "Bouw een CS PID controller")

# Na het instellen van deze variabele is het nodig een CMake Reset Cache uit te voeren :
# After setting this variable, a CMake cache reset is required :
# https://www.jetbrains.com/help/clion/cmake-cache.html

if (NOT InterfaceTaal)
	message(FATAL_ERROR "Geen taal gekozen : zet de voorkeurstaal!\nNo language selected. Please set the preferred language!\n")
else(NOT InterfaceTaal)
	message(STATUS "Taal/Language is : ${InterfaceTaal}")
	add_definitions(-DInterfaceTaal${InterfaceTaal})
endif (NOT InterfaceTaal)

if(NOT practicumopdracht)
	if(InterfaceTaal STREQUAL "Nederlands")
		message(FATAL_ERROR "Geen practicum gekozen : zet in dit dokument het gewenste practicum.")
	else(InterfaceTaal STREQUAL "Nederlands")
		message(FATAL_ERROR "Geen assigment selected : please set the assigment in this document.")
	endif(InterfaceTaal STREQUAL "Nederlands")
endif(NOT practicumopdracht)

string(TIMESTAMP DitJaar "%Y")
set(StdCopyrightString "(c)2006-${DitJaar} HAN Elektrotechniek/Embedded Systems Engineering.")

message("====================================================================================")
if(InterfaceTaal STREQUAL Nederlands)
	message("HAN Elektrotechniek/Embedded Systems Engineering ECSL/CSpr practicum.")
else(InterfaceTaal STREQUAL Nederlands)
	message("HAN Electrical Engineering/Embedded Systems Engineering ECSL practical work.")
endif(InterfaceTaal STREQUAL Nederlands)
message(${StdCopyrightString})
message("Docenten : ir H.A Arends en ir drs E.J Boks")
message("====================================================================================")

set(VAK "CS" CACHE STRING "CS practicum")
set(VERSIENUMMER "2.1" CACHE STRING "Versienummer")
set(VersieString "${VERSIENUMMER}")

option(CCACHEGEBRUIKT "Gebruik CCache" OFF)
option(CPPCHECKAAN "Gebruik CPP Check" OFF)
option(OverrideGCC_G++ "Gebruik C++ voor C" ON)
option(GebruikHALDrivers "Gebruik ST HAL bibliotheek" ON)
option(GebruikFreeRTOS "Gebruik FreeRTOS" ON)
option(GebruikOOFreeRTOS "Gebruik FreeRTOS OO lib" ON)

set(StudentRelDir ${PROJECT_SOURCE_DIR}/../STUDENT)
# Maak absoluut voor minder lang pad, vooral op windoze van belang.
get_filename_component(StudentDir "${StudentRelDir}"
                       REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")

#message(STATUS "EXT reldir = ${EXTERNALRELDIR} EXT dir = ${EXTERNALDIR}")
set(STM32BasisRelDir ${PROJECT_SOURCE_DIR}/../STM32Basis)
get_filename_component(STM32BasisDir "${STM32BasisRelDir}"
                       REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")

set(SoftwareBasisRelDir ${PROJECT_SOURCE_DIR}/../..)
get_filename_component(SoftwareBasisDir "${SoftwareBasisRelDir}"
                       REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")

# Zet de baudrate hier als de standaard 115200 NIET kan.
# Alter the baudrate here if the standaard 115200 is NOT feasible.
# Windows 10 workstation in B129/B133 can only do 9600 !
#set(STM32L432Nucleo32BaudRate 9600)


