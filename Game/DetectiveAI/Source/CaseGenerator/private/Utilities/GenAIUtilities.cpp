// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/GenAIUtilities.h"
#include "JsonObjectConverter.h"

bool UGenAIUtilities::JsonToUStruct(const FString& Content, const UScriptStruct* Schema, void* OutStruct)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// Iterate over the properties of the struct
		for (TFieldIterator<FProperty> It(Schema); It; ++It)
		{
			FProperty* Property = *It;
			FName PropertyName = Property->GetFName();

			// Check if the property is an enum
			if (FEnumProperty* EnumProp = CastField<FEnumProperty>(Property))
			{
				const UEnum* EnumDef = EnumProp->GetEnum();
				if (JsonObject->HasField(PropertyName.ToString()))
				{
					FString EnumValueString = JsonObject->GetStringField(PropertyName.ToString());
					int64 EnumValue = EnumDef->GetIndexByName(FName(*EnumValueString));

					// If invalid, set to default (0)
					if (EnumValue == INDEX_NONE)
					{
						EnumValue = 0; // Default to the first value in the enum
					}

					// Set the value in the struct
					void* ValuePtr = EnumProp->ContainerPtrToValuePtr<void>(OutStruct);
					EnumProp->GetUnderlyingProperty()->SetIntPropertyValue(ValuePtr, EnumValue);
				}
			}
		}

		// Continue with the usual JsonObjectToUStruct process
		if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), Schema, OutStruct, 0, 0))
		{
			return true;
		}
	}

	return false;
}


bool UGenAIUtilities::UStructToJsonObjectString(const UScriptStruct* InStructType, const void* InStruct, FString& OutJsonString)
{
	return FJsonObjectConverter::UStructToJsonObjectString(InStructType, InStruct, OutJsonString);
}
