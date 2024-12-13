// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/CaseGenLogger.h"

#include "NetworkMessage.h"
#include "Types/CommonCaseTypes.h"

UCaseGenLogger::UCaseGenLogger()
{
	FilePath = FPaths::ProjectDir() + TEXT("GenerationTimes4o.csv");
	if (!FPaths::FileExists(FilePath))
	{
		FString Header = "ActorGen Time,ClueGen Time,ContextGen Time,Total\n";
		FFileHelper::SaveStringToFile(Header, *FilePath);
	}
}
void UCaseGenLogger::GenStarted(EGenContext Context)
{
	switch (Context)
	{
	case EGenContext::Actor:
		ActorGenStartTime = FPlatformTime::Seconds();
		break;
	case EGenContext::Clue:
		ClueGenStartTime = FPlatformTime::Seconds();
		break;
	case EGenContext::Context:
		ContextGenStartTime = FPlatformTime::Seconds();
		break;
	}
}

void UCaseGenLogger::GenFinished(EGenContext Context)
{
	switch (Context)
	{
	case EGenContext::Actor:
		ActorGenDuration = FPlatformTime::Seconds() - ActorGenStartTime;
		break;
	case EGenContext::Clue:
		ClueGenDuration = FPlatformTime::Seconds() - ClueGenStartTime;
		break;
	case EGenContext::Context:
		ContextGenDuration = FPlatformTime::Seconds() - ContextGenStartTime;
		TotalGenDuration = FPlatformTime::Seconds() - ActorGenStartTime;
		WriteToCSV();
		break;
	}
}

void UCaseGenLogger::GenFailed()
{
	WriteToCSV();
}

void UCaseGenLogger::WriteToCSV() const
{
	FString Row = FString::Printf(TEXT("%.3f,%.3f,%.3f,%.3f\n"),
								  ActorGenDuration, ClueGenDuration,
								  ContextGenDuration, TotalGenDuration);
	
	FFileHelper::SaveStringToFile(Row, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}


void UCaseGenLogger::WriteCaseFile(const FCaseFile& CaseFile)
{
	FilePath = FPaths::ProjectDir() + TEXT("CaseFile.txt");

	FString Entry = FString::Printf(TEXT("%s\n%s"), FPlatformTime::StrTimestamp(), *CaseFile.ToString());
	FFileHelper::SaveStringToFile(Entry, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	
}