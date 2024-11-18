// SubmixBufferListener.h
#include "CoreMinimal.h"
#include "ISubmixBufferListener.h"
#include "Actors/SoundCaptureActor.h"


class FSubmixBufferListener : public ISubmixBufferListener, public TSharedFromThis<FSubmixBufferListener>
{
public:

	FSubmixBufferListener(TWeakObjectPtr<class ASoundCaptureActor> InOwner);

	virtual void OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples,
		int32 NumChannels, int32 SampleRate, double AudioClock) override
	{
		if (Owner.IsValid())
		{
			Owner->OnNewSubmixBuffer(OwningSubmix, AudioData, NumSamples, NumChannels, SampleRate, AudioClock);
		}
	}

private:
	TWeakObjectPtr<class ASoundCaptureActor> Owner;
};
