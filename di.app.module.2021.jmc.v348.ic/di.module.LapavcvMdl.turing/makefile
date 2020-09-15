#----------------------------------------------------------------------------------------
#	Framework name
#----------------------------------------------------------------------------------------

#	If there are more than one frameworks in the workspace, please tune the name below
#	to use the desired one
FWK_NAME			:= test.framework
#========================================================================================
# 	DO NOT MODIFY THIS SECTION !
#========================================================================================
PWD                 :=  $(shell sh -c 'pwd -W')
PRJ_DIR_X	       	:=	$(subst \,/,$(PWD))
PRJ_DIR	          	:=	$(subst //,, $(PRJ_DIR_X))
PRJ_NAME		    :=	$(notdir $(PRJ_DIR))

#	Path to the framework that this unit test is using
FWK_DIR				:=	$(subst //,/, $(dir $(PRJ_DIR))/$(FWK_NAME))

#========================================================================================
#	PROJECT SPECIFIC CONFIGURATIONS
#========================================================================================

#	Program identification
PROG_NAME			:="Turing Bookshelf"
#	Component under test name
CMP_NAME			:="TripMdl"
#	Component under test description
CMP_DESC			:="TripMdl Model"

#	Sub-folder of the unit test project directory, which will contain the source code
#	and/or objects of the tested unit
UNT_NAME			:=.
UNT_DIR				:=$(strip $(PRJ_DIR)/src/$(UNT_NAME))
UNT_DIR_RECURSIVE	:=NO

#	Sub-folder of the unit test project directory, which will contain the source code
#	and/or objects of the unit test procedure
TST_NAME			:=test.harness
TST_DIR				:=$(strip $(PRJ_DIR).test/$(TST_NAME))

#	Sub-folder of the unit test framework directory, which will contain the source code
#	and/or objects of the environment of the unit under test
ENV_NAME			:=env/diagnostics_env_ps
ENV_DIR				:=$(strip $(FWK_DIR)/$(ENV_NAME))


#	Sub-folder of the unit test project directory, which will cotains all XML and TXT
#	report files - for every configuration 0,1,2 ... and the summary XML
RPT_DIR				:=$(strip $(PRJ_DIR).test/results)

#	We need to have a unique report name, so we derive it from the unique project name
#	(Eclipse prjects have unique names within the same workspace)
#
#	MyUnit_Report.xml
#	MyUnit_Report_0.xml
#	MyUnit_Report_1.xml
RPT_NAME			:=Report_$(PRJ_NAME)

#	Folder with headers of the tester
TESTER_DIR			:=$(strip $(FWK_DIR)/tools/tester/)

#========================================================================================
#	UNIT SPECIFIC CONFIGURATIONS
#========================================================================================
# Language used for static/dynamic analysis: c or cpp
LANGUAGE			:=c

#	List of cpu that are used by this unit test
#	it must be a subset of CPU_SUPPORTED list, defined in the unit test framework 
CPU_USED			:=traveo

#	List of additional tools, used as separate targets 
TOOLS_USED			:=vcast polyspace qac coverity target simu debug doxygen

#----------------------------------------------------------------------------------------
#	Common preprocessor defines that are the same for all configurations
#----------------------------------------------------------------------------------------
DEFINES				:=UTEST
INCLUDES			:=$(TST_DIR) $(UNT_DIR) $(ENV_DIR) $(TESTER_DIR)
USER_LIBS           :=

#----------------------------------------------------------------------------------------
#	Unit source code locations
#----------------------------------------------------------------------------------------
	
UNT_SRC_DIR_C		:=$(UNT_DIR)
UNT_SRC_DIR_CXX		:=
UNT_SRC_DIR_AS		:=

UNT_SRC_EXCLUDE_C	:=
UNT_SRC_EXCLUDE_CXX	:=
UNT_SRC_EXCLUDE_AS	:=

#----------------------------------------------------------------------------------------
#	Test source code locations
#----------------------------------------------------------------------------------------
TST_SRC_DIR_C		:=$(TST_DIR)
TST_SRC_DIR_CXX		:=$(TST_DIR)

TST_SRC_EXCLUDE_C	:=
TST_SRC_EXCLUDE_CXX	:=

#----------------------------------------------------------------------------------------
#	Environment source code locations
#----------------------------------------------------------------------------------------
ENV_SRC_DIR_C		:=$(ENV_DIR)
ENV_SRC_DIR_CXX		:=

ENV_SRC_EXCLUDE_C	:=
ENV_SRC_EXCLUDE_CXX	:=

#----------------------------------------------------------------------------------------
#	Specific preprocessor defines for each configuration
#----------------------------------------------------------------------------------------
ifeq ($(cfg),CFG1)
DEFINES				+=
INCLUDES			+=
PREBUILDSTEPS    	:=
endif
ifeq ($(cfg),CFGn)
DEFINES				+=
INCLUDES			+=
PREBUILDSTEPS    	:=
endif

CFG_LIST_REPORTED   :=

REQUIREMENTS		:=""

#----------------------------------------------------------------------------------------
#	Specific compiler flags
#----------------------------------------------------------------------------------------
CFLAGS				:=-O0 -g -Wall -Wextra -Werror -c --pedantic-errors -std=c99 -Wno-unknown-pragmas -fmessage-length=0 -m32
CXXFLAGS			:=-O0 -g -Wall -Wextra -Werror -c --pedantic-errors -Wno-long-long -fmessage-length=0 -m32
ASFLAGS				:=-O0 -g -Wall -c -fmessage-length=0

TCFLAGS				:=-c -bsp generic -cpu=cortexr5 -fhard -thumb -align8 -Ospeed -OI -G -dwarf2 -C99 --ghstd=last --unknown_pragma_silent --quit_after_warnings
TCXXFLAGS			:=
TASFLAGS			:=

SCFLAGS				:=-c -Wall -wd"4820" -WX -Od -nologo -MTd -Zi -Gm
SCXXFLAGS			:=
SASFLAGS			:=

#========================================================================================
# 	DO NOT MODIFY THIS SECTION !
#========================================================================================
include  $(FWK_DIR)/common.mk

dummy:

.PHONY: dummy
