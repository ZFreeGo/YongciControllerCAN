#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=DeviceNet/DeviceNet.c Action.c AdcSample.c CRC16.c DeviceIO.c RtuFrame.c SwtichCondition.c Timer.c Usart.c main.c traps.c yongci.c DeviceParameter.c CAN.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/DeviceNet/DeviceNet.o ${OBJECTDIR}/Action.o ${OBJECTDIR}/AdcSample.o ${OBJECTDIR}/CRC16.o ${OBJECTDIR}/DeviceIO.o ${OBJECTDIR}/RtuFrame.o ${OBJECTDIR}/SwtichCondition.o ${OBJECTDIR}/Timer.o ${OBJECTDIR}/Usart.o ${OBJECTDIR}/main.o ${OBJECTDIR}/traps.o ${OBJECTDIR}/yongci.o ${OBJECTDIR}/DeviceParameter.o ${OBJECTDIR}/CAN.o
POSSIBLE_DEPFILES=${OBJECTDIR}/DeviceNet/DeviceNet.o.d ${OBJECTDIR}/Action.o.d ${OBJECTDIR}/AdcSample.o.d ${OBJECTDIR}/CRC16.o.d ${OBJECTDIR}/DeviceIO.o.d ${OBJECTDIR}/RtuFrame.o.d ${OBJECTDIR}/SwtichCondition.o.d ${OBJECTDIR}/Timer.o.d ${OBJECTDIR}/Usart.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/traps.o.d ${OBJECTDIR}/yongci.o.d ${OBJECTDIR}/DeviceParameter.o.d ${OBJECTDIR}/CAN.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/DeviceNet/DeviceNet.o ${OBJECTDIR}/Action.o ${OBJECTDIR}/AdcSample.o ${OBJECTDIR}/CRC16.o ${OBJECTDIR}/DeviceIO.o ${OBJECTDIR}/RtuFrame.o ${OBJECTDIR}/SwtichCondition.o ${OBJECTDIR}/Timer.o ${OBJECTDIR}/Usart.o ${OBJECTDIR}/main.o ${OBJECTDIR}/traps.o ${OBJECTDIR}/yongci.o ${OBJECTDIR}/DeviceParameter.o ${OBJECTDIR}/CAN.o

# Source Files
SOURCEFILES=DeviceNet/DeviceNet.c Action.c AdcSample.c CRC16.c DeviceIO.c RtuFrame.c SwtichCondition.c Timer.c Usart.c main.c traps.c yongci.c DeviceParameter.c CAN.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4011
MP_LINKER_FILE_OPTION=,--script=p30F4011.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/DeviceNet/DeviceNet.o: DeviceNet/DeviceNet.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/DeviceNet" 
	@${RM} ${OBJECTDIR}/DeviceNet/DeviceNet.o.d 
	@${RM} ${OBJECTDIR}/DeviceNet/DeviceNet.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DeviceNet/DeviceNet.c  -o ${OBJECTDIR}/DeviceNet/DeviceNet.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DeviceNet/DeviceNet.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/DeviceNet/DeviceNet.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Action.o: Action.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Action.o.d 
	@${RM} ${OBJECTDIR}/Action.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Action.c  -o ${OBJECTDIR}/Action.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Action.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/Action.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/AdcSample.o: AdcSample.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AdcSample.o.d 
	@${RM} ${OBJECTDIR}/AdcSample.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  AdcSample.c  -o ${OBJECTDIR}/AdcSample.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/AdcSample.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/AdcSample.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/CRC16.o: CRC16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CRC16.o.d 
	@${RM} ${OBJECTDIR}/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CRC16.c  -o ${OBJECTDIR}/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/CRC16.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/CRC16.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DeviceIO.o: DeviceIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DeviceIO.o.d 
	@${RM} ${OBJECTDIR}/DeviceIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DeviceIO.c  -o ${OBJECTDIR}/DeviceIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DeviceIO.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/DeviceIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/RtuFrame.o: RtuFrame.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RtuFrame.o.d 
	@${RM} ${OBJECTDIR}/RtuFrame.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RtuFrame.c  -o ${OBJECTDIR}/RtuFrame.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RtuFrame.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/RtuFrame.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/SwtichCondition.o: SwtichCondition.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SwtichCondition.o.d 
	@${RM} ${OBJECTDIR}/SwtichCondition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SwtichCondition.c  -o ${OBJECTDIR}/SwtichCondition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SwtichCondition.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/SwtichCondition.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Timer.o: Timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Timer.o.d 
	@${RM} ${OBJECTDIR}/Timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timer.c  -o ${OBJECTDIR}/Timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/Timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Usart.o: Usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Usart.o.d 
	@${RM} ${OBJECTDIR}/Usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Usart.c  -o ${OBJECTDIR}/Usart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Usart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/Usart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/traps.o: traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/traps.o.d 
	@${RM} ${OBJECTDIR}/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  traps.c  -o ${OBJECTDIR}/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/yongci.o: yongci.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/yongci.o.d 
	@${RM} ${OBJECTDIR}/yongci.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  yongci.c  -o ${OBJECTDIR}/yongci.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/yongci.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/yongci.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DeviceParameter.o: DeviceParameter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DeviceParameter.o.d 
	@${RM} ${OBJECTDIR}/DeviceParameter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DeviceParameter.c  -o ${OBJECTDIR}/DeviceParameter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DeviceParameter.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/DeviceParameter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/CAN.o: CAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CAN.o.d 
	@${RM} ${OBJECTDIR}/CAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CAN.c  -o ${OBJECTDIR}/CAN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/CAN.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/CAN.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/DeviceNet/DeviceNet.o: DeviceNet/DeviceNet.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/DeviceNet" 
	@${RM} ${OBJECTDIR}/DeviceNet/DeviceNet.o.d 
	@${RM} ${OBJECTDIR}/DeviceNet/DeviceNet.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DeviceNet/DeviceNet.c  -o ${OBJECTDIR}/DeviceNet/DeviceNet.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DeviceNet/DeviceNet.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/DeviceNet/DeviceNet.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Action.o: Action.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Action.o.d 
	@${RM} ${OBJECTDIR}/Action.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Action.c  -o ${OBJECTDIR}/Action.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Action.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/Action.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/AdcSample.o: AdcSample.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AdcSample.o.d 
	@${RM} ${OBJECTDIR}/AdcSample.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  AdcSample.c  -o ${OBJECTDIR}/AdcSample.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/AdcSample.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/AdcSample.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/CRC16.o: CRC16.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CRC16.o.d 
	@${RM} ${OBJECTDIR}/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CRC16.c  -o ${OBJECTDIR}/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/CRC16.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/CRC16.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DeviceIO.o: DeviceIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DeviceIO.o.d 
	@${RM} ${OBJECTDIR}/DeviceIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DeviceIO.c  -o ${OBJECTDIR}/DeviceIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DeviceIO.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/DeviceIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/RtuFrame.o: RtuFrame.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RtuFrame.o.d 
	@${RM} ${OBJECTDIR}/RtuFrame.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RtuFrame.c  -o ${OBJECTDIR}/RtuFrame.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RtuFrame.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/RtuFrame.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/SwtichCondition.o: SwtichCondition.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SwtichCondition.o.d 
	@${RM} ${OBJECTDIR}/SwtichCondition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SwtichCondition.c  -o ${OBJECTDIR}/SwtichCondition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SwtichCondition.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/SwtichCondition.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Timer.o: Timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Timer.o.d 
	@${RM} ${OBJECTDIR}/Timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timer.c  -o ${OBJECTDIR}/Timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Timer.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/Timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Usart.o: Usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Usart.o.d 
	@${RM} ${OBJECTDIR}/Usart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Usart.c  -o ${OBJECTDIR}/Usart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Usart.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/Usart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/traps.o: traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/traps.o.d 
	@${RM} ${OBJECTDIR}/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  traps.c  -o ${OBJECTDIR}/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/traps.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/yongci.o: yongci.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/yongci.o.d 
	@${RM} ${OBJECTDIR}/yongci.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  yongci.c  -o ${OBJECTDIR}/yongci.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/yongci.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/yongci.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DeviceParameter.o: DeviceParameter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DeviceParameter.o.d 
	@${RM} ${OBJECTDIR}/DeviceParameter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DeviceParameter.c  -o ${OBJECTDIR}/DeviceParameter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DeviceParameter.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/DeviceParameter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/CAN.o: CAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CAN.o.d 
	@${RM} ${OBJECTDIR}/CAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CAN.c  -o ${OBJECTDIR}/CAN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/CAN.o.d"        -g -omf=elf -legacy-libc  -O1 -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99
	@${FIXDEPS} "${OBJECTDIR}/CAN.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -legacy-libc   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -legacy-libc  -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/YongciContronerCAN.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
