// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEHttpGPT

#include "Utils/HttpGPTHelper.h"
#include "HttpGPTInternalFuncs.h"

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(HttpGPTHelper)
#endif

const FName UHttpGPTHelper::ModelToName(const EHttpGPTChatModel Model)
{
	switch (Model)
	{
	case EHttpGPTChatModel::gpt4o:
		return "gpt-4o";

	case EHttpGPTChatModel::gpt4omini:
		return "gpt-4o-mini";

	case EHttpGPTChatModel::gpt4:
		return "gpt-4";

	case EHttpGPTChatModel::gpt432k:
		return "gpt-4-32k";

	case EHttpGPTChatModel::gpt35turbo:
		return "gpt-3.5-turbo";

	case EHttpGPTChatModel::gpt35turbo16k:
		return "gpt-3.5-turbo-16k";

	case EHttpGPTChatModel::textdavinci003:
		return "text-davinci-003";

	case EHttpGPTChatModel::textdavinci002:
		return "text-davinci-002";

	case EHttpGPTChatModel::codedavinci002:
		return "code-davinci-002";

	default: break;
	}

	return NAME_None;
}

const FString UHttpGPTHelper::ModelToString(const EHttpGPTVoiceModel Model)
{
	switch (Model)
	{
	case EHttpGPTVoiceModel::tts1:
		return "tts-1";

	case EHttpGPTVoiceModel::tts1_hd:
		return "tts-1-hd";
	}
	return "";
}

const FString UHttpGPTHelper::VoiceToString(const EHttpGPTSynthVoice Voice)
{
	switch (Voice)
	{
	case EHttpGPTSynthVoice::alloy:
		return "alloy";
	case EHttpGPTSynthVoice::echo:
		return "echo";
	case EHttpGPTSynthVoice::fable:
		return "fable";
	case EHttpGPTSynthVoice::nova:
		return "nova";
	case EHttpGPTSynthVoice::onyx:
		return "onyx";
	case EHttpGPTSynthVoice::shimmer:
		return "shimmer";
	}
	return "";
}
const FString UHttpGPTHelper::OutputFormatToString(const EHttpGPTVoiceOutputFormat OutputFormat)
{
	switch (OutputFormat)
	{
	case EHttpGPTVoiceOutputFormat::Mp3:
		return "mp3";
	case EHttpGPTVoiceOutputFormat::Pcm:
		return  "pcm";
	case EHttpGPTVoiceOutputFormat::Opus:
		return  "opus";
	case EHttpGPTVoiceOutputFormat::Aac:
		return  "aac";
	case EHttpGPTVoiceOutputFormat::Flac:
		return  "flac";
	case EHttpGPTVoiceOutputFormat::Wav:
		return  "wav";
	default:
		return "mp3";
	}
}


const EHttpGPTChatModel UHttpGPTHelper::NameToModel(const FName Model)
{
	if (Model.IsEqual("gpt-4o", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::gpt4o;
	}
	if (Model.IsEqual("gpt-4o-mini", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::gpt4omini;
	}
	if (Model.IsEqual("gpt-4", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::gpt4;
	}
	if (Model.IsEqual("gpt-4-32k", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::gpt432k;
	}
	if (Model.IsEqual("gpt-3.5-turbo", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::gpt35turbo;
	}
	if (Model.IsEqual("gpt-3.5-turbo-16k", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::gpt35turbo16k;
	}
	if (Model.IsEqual("text-davinci-003", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::textdavinci003;
	}
	if (Model.IsEqual("text-davinci-002", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::textdavinci002;
	}
	if (Model.IsEqual("code-davinci-002", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatModel::codedavinci002;
	}

	return EHttpGPTChatModel::gpt35turbo;
}

const FName UHttpGPTHelper::RoleToName(const EHttpGPTChatRole Role)
{
	switch (Role)
	{
	case EHttpGPTChatRole::Assistant:
		return "assistant";

	case EHttpGPTChatRole::User:
		return "user";

	case EHttpGPTChatRole::System:
		return "system";

	case EHttpGPTChatRole::Function:
		return "function";

	default:
		break;
	}

	return NAME_None;
}

const EHttpGPTChatRole UHttpGPTHelper::NameToRole(const FName Role)
{
	if (Role.IsEqual("user", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatRole::User;
	}
	if (Role.IsEqual("assistant", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatRole::Assistant;
	}
	if (Role.IsEqual("system", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatRole::System;
	}
	if (Role.IsEqual("function", ENameCase::IgnoreCase))
	{
		return EHttpGPTChatRole::Function;
	}

	return EHttpGPTChatRole::User;
}

const FName UHttpGPTHelper::PropertyTypeToName(const EHttpGPTPropertyType Type)
{
	switch (Type)
	{
	case EHttpGPTPropertyType::Boolean:
		return "bool";
	case EHttpGPTPropertyType::Number:
		return "number";
	case EHttpGPTPropertyType::String:
		return "string";
	case EHttpGPTPropertyType::Integer:
		return "integer";
	case EHttpGPTPropertyType::Object:
		return "object";
	case EHttpGPTPropertyType::Array:
		return "array";
	case EHttpGPTPropertyType::Enum:
		return "enum";
	default:
		break;
	}

	return NAME_None;
}


const EHttpGPTPropertyType UHttpGPTHelper::NameToPropertyType(const FName Type)
{
	if (Type.IsEqual("bool", ENameCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::Boolean;
	}
	if (Type.IsEqual("number", ENameCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::Number;
	}
	if (Type.IsEqual("string", ENameCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::String;
	}
	if (Type.IsEqual("int", ENameCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::Integer;
	}
	if (Type.IsEqual("array", ENameCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::Array;
	}

	return EHttpGPTPropertyType::Boolean;
}

const EHttpGPTPropertyType UHttpGPTHelper::CPPToPropertyType(const FString Type)
{
	if(Type.Contains("string", ESearchCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::String;
	}
	if(Type.Contains("int", ESearchCase::IgnoreCase))
	{
	return EHttpGPTPropertyType::Integer;
	}
	if(Type.Contains("float", ESearchCase::IgnoreCase) || Type.Contains("double", ESearchCase::IgnoreCase))
	{
	return EHttpGPTPropertyType::Number;
	}
	if(Type.Contains("bool", ESearchCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::Boolean;
	}
	if (Type.Contains("array", ESearchCase::IgnoreCase))
	{
		return EHttpGPTPropertyType::Array;
	}
	if(!Type.IsEmpty() && Type[0] == 'E')
	{
		return EHttpGPTPropertyType::String;
	}

	return EHttpGPTPropertyType::Object;
}

const TArray<FName> UHttpGPTHelper::GetAvailableGPTModels()
{
	TArray<FName> Output;

	for (uint8 Iterator = static_cast<uint8>(EHttpGPTChatModel::gpt4); Iterator <= static_cast<uint8>(EHttpGPTChatModel::codedavinci002); ++Iterator)
	{
		if (const FName ModelName = ModelToName(static_cast<EHttpGPTChatModel>(Iterator)); !HttpGPT::Internal::HasEmptyParam(ModelName))
		{
			Output.Add(ModelName);
		}
	}

	return Output;
}

const FString UHttpGPTHelper::GetEndpointForModel(const EHttpGPTChatModel Model, const bool bIsAzureOpenAI, const FString& AzureOpenAIAPIVersion)
{
	switch (Model)
	{
	case EHttpGPTChatModel::gpt4o:
	case EHttpGPTChatModel::gpt4omini:
	case EHttpGPTChatModel::gpt4:
	case EHttpGPTChatModel::gpt432k:
	case EHttpGPTChatModel::gpt35turbo:
	case EHttpGPTChatModel::gpt35turbo16k:
		{
			if (bIsAzureOpenAI)
			{
				return FString::Format(
					TEXT("/openai/deployments/{0}/chat/completions?api-version={1}"), {ModelToName(Model).ToString(), AzureOpenAIAPIVersion});
			}
			return "v1/chat/completions";
		}

	case EHttpGPTChatModel::textdavinci003:
	case EHttpGPTChatModel::textdavinci002:
	case EHttpGPTChatModel::codedavinci002:
		{
			if (bIsAzureOpenAI)
			{
				return FString::Format(
					TEXT("/openai/deployments/{0}/completions?api-version={1}"), {ModelToName(Model).ToString(), AzureOpenAIAPIVersion});
			}
			return "v1/completions";
		}

	default: break;
	}

	return FString();
}

const bool UHttpGPTHelper::ModelSupportsChat(const EHttpGPTChatModel Model)
{
	switch (Model)
	{
	case EHttpGPTChatModel::gpt4:
	case EHttpGPTChatModel::gpt4o:
	case EHttpGPTChatModel::gpt4omini:
	case EHttpGPTChatModel::gpt432k:
	case EHttpGPTChatModel::gpt35turbo:
	case EHttpGPTChatModel::gpt35turbo16k:
		return true;

	case EHttpGPTChatModel::textdavinci003:
	case EHttpGPTChatModel::textdavinci002:
	case EHttpGPTChatModel::codedavinci002:
		return false;

	default: break;
	}

	return false;
}

const bool UHttpGPTHelper::ModelSupportsStructuredResponse(const EHttpGPTChatModel Model)
{
	switch (Model)
	{
	case EHttpGPTChatModel::gpt4o:
	case EHttpGPTChatModel::gpt4omini:
		return true;
	case EHttpGPTChatModel::gpt4:
	case EHttpGPTChatModel::gpt432k:
	case EHttpGPTChatModel::gpt35turbo:
	case EHttpGPTChatModel::gpt35turbo16k:
	case EHttpGPTChatModel::textdavinci003:
	case EHttpGPTChatModel::textdavinci002:
	case EHttpGPTChatModel::codedavinci002:
		return false;

	default: break;
	}

	return false;
}

const FName UHttpGPTHelper::SizeToName(const EHttpGPTImageSize Size)
{
	switch (Size)
	{
	case EHttpGPTImageSize::x256:
		return "256x256";

	case EHttpGPTImageSize::x512:
		return "512x512";

	case EHttpGPTImageSize::x1024:
		return "1024x1024";

	default:
		break;
	}

	return NAME_None;
}

const EHttpGPTImageSize UHttpGPTHelper::NameToSize(const FName Size)
{
	if (Size.IsEqual("256x256", ENameCase::IgnoreCase))
	{
		return EHttpGPTImageSize::x256;
	}
	if (Size.IsEqual("512x512", ENameCase::IgnoreCase))
	{
		return EHttpGPTImageSize::x512;
	}
	if (Size.IsEqual("1024x1024", ENameCase::IgnoreCase))
	{
		return EHttpGPTImageSize::x1024;
	}

	return EHttpGPTImageSize::x256;
}

const FName UHttpGPTHelper::FormatToName(const EHttpGPTResponseFormat Format)
{
	switch (Format)
	{
	case EHttpGPTResponseFormat::url:
		return "url";

	case EHttpGPTResponseFormat::b64_json:
		return "b64_json";

	default:
		break;
	}

	return NAME_None;
}

const EHttpGPTResponseFormat UHttpGPTHelper::NameToFormat(const FName Format)
{
	if (Format.IsEqual("url", ENameCase::IgnoreCase))
	{
		return EHttpGPTResponseFormat::url;
	}
	if (Format.IsEqual("b64_json", ENameCase::IgnoreCase))
	{
		return EHttpGPTResponseFormat::b64_json;
	}

	return EHttpGPTResponseFormat::url;
}
