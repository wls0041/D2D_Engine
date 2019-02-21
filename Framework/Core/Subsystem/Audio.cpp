#include "Framework.h"
#include "Audio.h"

Audio::Audio(Context * context)
	: ISubsystem(context)
	, system(nullptr)
	, maxChannel(32)
	, distanceFactor(1.0f)
	, bInitialized(false)
	, check(FMOD_OK)
{
}

Audio::~Audio()
{
	if (!bInitialized)
		return;

	check = system->close();
	assert(check == FMOD_OK);

	check = system->release();
	assert(check == FMOD_OK);
}

const bool Audio::Initialize()
{
	if (bInitialized)
		return false;

	//FMOD 장치 생성
	check = System_Create(&system);
	assert(check == FMOD_OK);

	//FMOD 버전 체크
	uint version = 0;
	check = system->getVersion(&version);
	assert(check == FMOD_OK);
	assert(version >= FMOD_VERSION);

	//사운드 카드 장치가 있는지 체크
	int driverCount = 0;
	check = system->getNumDrivers(&driverCount);
	assert(check == FMOD_OK);
	assert(driverCount > 0);

	//FMOD System 초기화
	check = system->init(maxChannel, FMOD_INIT_NORMAL, nullptr);
	assert(check == FMOD_OK);

	//FMOD 3D Settings
	check = system->set3DSettings
	(
		1.0f,
		distanceFactor,
		0.0f
	);
	assert(check == FMOD_OK);

	bInitialized = true;

	return true;
}

void Audio::Update()
{
	if (!bInitialized)
		return;

	check = system->update();
	assert(check == FMOD_OK);
}
