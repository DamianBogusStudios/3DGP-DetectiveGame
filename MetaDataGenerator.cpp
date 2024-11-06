// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities/MetaDataGenerator.h"
#include "Types/CommonCaseTypes.h"

void UMetaDataGenerator::SetTableData()
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/todo"));
	
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
				
		for (TFieldIterator<FProperty> PropIt(Struct); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			if (Property && Property->HasMetaData(TEXT("LLMDescription")))
			{
				FString Description = Property->GetMetaData(TEXT("LLMDescription"));

				
				FDescriptionTableRow RowEntry;
				RowEntry.PropertyKey = Struct->GetName() + "_" + Property->GetName();
				RowEntry.Description = *Description;
				
				DataTable->AddRow(FName(*Property->GetName()), RowEntry);
				
				UE_LOG(LogTemp, Log, TEXT("Added Row Property: %s, LLMDescription: %s"), *Property->GetName(), *Description);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Data Table populated successfully."));
}
