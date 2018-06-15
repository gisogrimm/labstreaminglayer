#ifndef LiveAmp_H
#define LiveAmp_H

// Header file for higher level LiveAmp connection functions

// LiveAmp API
#include "Amplifier_LIB.h"
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>




class LiveAmp {

private:
	// data get from device (defined in construction)
	// these are initialized in the constructor
	HANDLE h;                        // device handle
	std::string serialNumber;        // serial number
	int availableChannels;           // number of available channels
	int usableChannels;				 // whether we are liveamp 8, 16, 32, 64
	int availableModules;            // use to detect STE box
	bool bHasSTE;                    // actual flag for STE connected
	int STEIdx;                      // index of which module is the STE

	// these are set later on by the user
	float samplingRate;
	int recordingMode;               // defaults to record mode

	// set during configuration
	typedef struct _channelInfo {
		int dataType;
		float resolution;
		int channelType;
	}t_channelInfo;
	// int dataTypeArray[100];
	t_channelInfo channelInfoArray[100];
	int sampleSize;


	// flexible? channel access arrays
	std::vector<int> channelIndexes;  
	std::vector<int> eegIndices;
	std::vector<int> bipolarIndices;
	std::vector<int> auxIndices;
	std::vector<int> accIndices;
	std::vector<int> trigIndices;

	int enabledChannelCnt;
	bool bIsClosed;
	

public:
	// constructor
	// initialize by at least the serial number and a container for the enumerated devices
	LiveAmp::LiveAmp(std::string serialNumberIn, float samplingRateIn = 500, bool useSim = false, int recordingModeIn = RM_NORMAL);
	//LiveAmp::LiveAmp (void);
	// destructor
	~LiveAmp();
	
	// get the serial numbers and channel counts of all available liveamps
	static void enumerate(std::vector<std::pair<std::string, int>> &ampData, bool useSim=false);

	// close live amp device
	void close();

	// enable requested channels: for now acc and aux are all or nothing, triggers are always on, and eeg channels can be selected
	void enableChannels(const std::vector<int>& eegIndicesIn, const std::vector<int>& bipolarIndicesIn, const std::vector<int>& auxIndicesIn, bool accEnable);

	// activate the configured device with enabled channels
	void startAcquisition(void);

	// activate the configured device with enabled channels
	void stopAcquisition(void);

	// get data from device
	int64_t pullAmpData(BYTE* buffer, int bufferSize);

	// push it into a vector TODO: make this a template to support any buffer type
	void pushAmpData(BYTE* buffer, int bufferSize, int64_t samplesRead, std::vector<std::vector<float>> &outData);
	
	// set the output trigger output mode
	void setOutTriggerMode(t_TriggerOutputMode mode, int syncPin, int freq, int pulseWidth);

	// public data access methods 	
	inline float             getSamplingRate(void){return samplingRate;}
	inline HANDLE            getHandle(void) { if (h == NULL)return NULL;else return h; }
	inline std::string&      getSerialNumber(void){return serialNumber;}
	inline int               getAvailableChannels(void){return availableChannels;}
	inline int               getRecordingMode(void){return recordingMode;}
	inline std::vector<int>& getEEGIndices(void){return eegIndices;}
	inline std::vector<int>& getBipolarIndices(void){return bipolarIndices;}
	inline std::vector<int>& getAuxIndices(void){return auxIndices;}
	inline std::vector<int>& getAccIndices(void){return accIndices;}
	inline std::vector<int>& getTrigIndices(void){return trigIndices;}
	inline int               getEnabledChannelCnt(void){return enabledChannelCnt;}
	//inline int*              getDataTypeArray(void){return dataTypeArray;}  
	inline int               getSampleSize(void){return sampleSize;}
	inline int				 getUsableChannels(void){return usableChannels;}
	inline bool				 isClosed(void){return bIsClosed;}
	inline bool              hasSTE(void) { return bHasSTE; }
};

#endif //LiveAmp_H