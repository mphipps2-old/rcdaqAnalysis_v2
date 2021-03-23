Quick Start:
1) Open RunParameters and set run conditions.
2) Run: 
(From rcdaqAnalysis directory) "root -l", 
(within ROOT session) ".L libAnalysis.so", 
"real_init", 
"pfileopen("filename")" (for beamtest data) or "poncsopen("filename") for in-lab data or "rcdaqopen()" for real-time monitoring
"prun()" (to process entire file) or "prun(nevents)" to see nth event



DAQ Concept

This program is meant to interface with RCDAQ and be used for monitoring and first level analysis. The motivating idea is that there are certain basic processing routines required for all forms of analysis -- across almost all readout devices and detectors. Those processes should be consolidated into a single series of routines, from which all specialized analysis can proceed. I'll briefly describe each of the modules and input files needed for this program.


FAQ

Question: "I'm using a new detector/readout board/reaout system. What do I need to change?"
Answer: Almost all the configurable parameters are within the configuration file ("RunParameters.txt"). If you are adding a new readout system, you need to go to Processor::UnpackData() and define the packet number and iValue/rValue you receive from RCDAQ. You also may to go to the Processor constructor and define a new MaxADC value for your readout system. You will also have to add a mapping text file (you can use one of the mapping generator c scripts as a template to automate this process) and a calibration text file (if you want to calibrate each channel. Everything else should be transfer over. If it doesn't, you can contact me: mphipps2@illinois.edu.


Description of the DAQ Modules

Configuration file:  "RunParameters.txt" defines all the parameters you need for either monitoring or analysis. The program itself will read this file and adjust the output accordingly. 

Front level script: "Analysis.cc". This is where the flow of the program is set and is intended to be as short and concise as possible. All routines are performed elsewhere.

Processor: This module controls the primary analysis. It unpacks the data, organizes it, and applies basic cuts and corrections. The cluster and hit modules are filled from this module.

Configuration: This module reads the configuration file.

TreeManager: This module is intended to be used for the most basic forms of analysis. It writes data from the processor module to TTrees that can be used in subsequent analysis. The TTree data comes in two forms: 1) predefined (basic 1d) cluster/hit trees and 2) raw unclustered hits. If you want to expand on the current cluster definitions you should use the former. If you want to modify the conditions in this program, you can use the latter. 

Mapping: This should be done by listing listing the channels, pad numbers, X coordinates and y coordinates in a text file. These can be automatically generated through routines similar to the ones found in: "StruckTPCMappingGenerator.C" and "ChevronMappingGenerator.C". These routines are executed in CINT (ROOT) by loading the file and then executing function.

Note: This program is meant to be run during a ROOT session, while interfacing with RCDAQ. We have found that compiling code outside of ROOT is faster and more transparent (for debugging). For that reason, all subsequent analysis should be run through g++ or a similar compiler (with the needed ROOT libraries and class definitions linked). 


