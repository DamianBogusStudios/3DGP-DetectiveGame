// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaDataGenerator.h"
#include "Types/CommonCaseTypes.h"


void UMetaDataGenerator::SetTableData()
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/DT_MetaData.DT_MetaData'"));
	
	if (!DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Data Table not found."));
		return;
	}

	DataTable->EmptyTable();

	for (TObjectIterator<UScriptStruct> It; It; ++It)
	{
		UScriptStruct* Struct = *It;
		UE_LOG(LogTemp, Log, TEXT("Inspecting Struct: %s"), *Struct->GetName());

		if(Struct->HasMetaData(TEXT("LLMDescription")))
		{
			FString Description = Struct->GetMetaData(TEXT("LLMDescription"));

			FDescriptionTableRow RowEntry;
			RowEntry.Description = *Description;
			
			DataTable->AddRow(FName(Struct->GetName()), RowEntry);
		}
		
		for (TFieldIterator<FProperty> PropIt(Struct); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			if (Property && Property->HasMetaData(TEXT("LLMDescription")))
			{
				FString Description = Property->GetMetaData(TEXT("LLMDescription"));

				
				FDescriptionTableRow RowEntry;
				RowEntry.Description = *Description;
				
				DataTable->AddRow(FName(Struct->GetName() + "_" + Property->GetName()), RowEntry);
				
				UE_LOG(LogTemp, Log, TEXT("Added Row Property: %s, LLMDescription: %s"), *Property->GetName(), *Description);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Data Table populated successfully."));
}
