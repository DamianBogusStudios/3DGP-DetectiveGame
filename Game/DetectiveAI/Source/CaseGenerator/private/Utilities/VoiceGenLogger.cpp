// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/VoiceGenLogger.h"

UVoiceGenLogger::UVoiceGenLogger()
{
	FilePath = FPaths::ProjectDir() + TEXT("DialogueGenTimes.csv");
	if (!FPaths::FileExists(FilePath))
	{
		FString Header = "Response Length, Response,Voice\n";
		FFileHelper::SaveStringToFile(Header, *FilePath);
	}
}
void UVoiceGenLogger::ResponseRequested()
{
	ResponseStart = FPlatformTime::Seconds();
}
void UVoiceGenLogger::ResponseReceived(int Length)
{
	ResponseLength = Length;
	ResponseDuration = FPlatformTime::Seconds() - ResponseStart;
}
void UVoiceGenLogger::VoiceRequested()
{
	VoiceStart = FPlatformTime::Seconds();
}
void UVoiceGenLogger::VoiceReceived()
{
	VoiceDuration = FPlatformTime::Seconds() - VoiceStart;
	WriteToCSV();
}

void UVoiceGenLogger::WriteToCSV() const
{
	FString Row = FString::Printf(TEXT("%d,%.2f,%.2f\n"),
								  ResponseLength, ResponseDuration,VoiceDuration);
	
	FFileHelper::SaveStringToFile(Row, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}
