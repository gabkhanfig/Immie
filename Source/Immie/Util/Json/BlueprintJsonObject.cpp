// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintJsonObject.h"
#include <Immie/ImmieCore.h>

FJsonArrayBP::FJsonArrayBP()
{
}

FJsonArrayBP::FJsonArrayBP(const TArray<TSharedPtr<FJsonValue>>& Array)
{
	JsonData = Array;
}

FJsonArrayBP FJsonArrayBP::FromArray(const TArray<TSharedPtr<FJsonValue>>& Array)
{
	FJsonArrayBP JsonArray;
	JsonArray.JsonData = Array;
	return JsonArray;
}

int FJsonArrayBP::Num()
{
	return JsonData.Num();
}

TArray<bool> FJsonArrayBP::GetBoolArray(const FString& ElementName) const
{
	const int ElementCount = JsonData.Num();
	TArray<bool> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		bool Temp;
		if (JsonData[i]->AsObject()->TryGetBoolField(ElementName, Temp)) {
			Array.Add(Temp);
		}
	}
	return Array;
}

void FJsonArrayBP::AddBoolElement(const FString& ElementName, bool Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetBoolField(ElementName, Value);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetBoolArray(const FString& ElementName, const TArray<bool>& Array)
{
	const int ElementCount = Array.Num();
	JsonData.Empty();
	JsonData.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		AddBoolElement(ElementName, Array[i]);
	}
}

TArray<int> FJsonArrayBP::GetIntegerArray(const FString& ElementName) const
{
	const int ElementCount = JsonData.Num();
	TArray<int> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		int Temp;
		if (JsonData[i]->AsObject()->TryGetNumberField(ElementName, Temp)) {
			Array.Add(Temp);
		}
	}
	return Array;
}

void FJsonArrayBP::AddIntegerElement(const FString& ElementName, int Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetNumberField(ElementName, Value);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetIntegerArray(const FString& ElementName, const TArray<int>& Array)
{
	const int ElementCount = Array.Num();
	JsonData.Empty();
	JsonData.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		AddIntegerElement(ElementName, Array[i]);
	}
}

TArray<int64> FJsonArrayBP::GetBigIntegerArray(const FString& ElementName) const
{
	const int ElementCount = JsonData.Num();
	TArray<int64> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		int64 Temp;
		if (JsonData[i]->AsObject()->TryGetNumberField(ElementName, Temp)) {
			Array.Add(Temp);
		}
	}
	return Array;
}

void FJsonArrayBP::AddBigIntegerElement(const FString& ElementName, int64 Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetNumberField(ElementName, Value);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetBigIntegerArray(const FString& ElementName, const TArray<int64>& Array)
{
	const int ElementCount = Array.Num();
	JsonData.Empty();
	JsonData.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		AddBigIntegerElement(ElementName, Array[i]);
	}
}

TArray<float> FJsonArrayBP::GetFloatArray(const FString& ElementName) const
{
	const int ElementCount = JsonData.Num();
	TArray<float> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		float Temp;
		if (JsonData[i]->AsObject()->TryGetNumberField(ElementName, Temp)) {
			Array.Add(Temp);
		}
	}
	return Array;
}

void FJsonArrayBP::AddFloatElement(const FString& ElementName, float Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetNumberField(ElementName, Value);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetFloatArray(const FString& ElementName, const TArray<float>& Array)
{
	const int ElementCount = Array.Num();
	JsonData.Empty();
	JsonData.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		AddFloatElement(ElementName, Array[i]);
	}
}

TArray<FString> FJsonArrayBP::GetStringArray(const FString& ElementName) const
{
	const int ElementCount = JsonData.Num();
	TArray<FString> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		FString Temp;
		if (JsonData[i]->AsObject()->TryGetStringField(ElementName, Temp)) {
			Array.Add(std::move(Temp));
		}
	}
	return Array;
}

void FJsonArrayBP::AddStringElement(const FString& ElementName, const FString& Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetStringField(ElementName, Value);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetStringArray(const FString& ElementName, const TArray<FString>& Array)
{
	const int ElementCount = Array.Num();
	JsonData.Empty();
	JsonData.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		AddStringElement(ElementName, Array[i]);
	}
}

TArray<TSharedPtr<FJsonObject>> FJsonArrayBP::GetObjectArray() const
{
	const int ElementCount = JsonData.Num();
	TArray<TSharedPtr<FJsonObject>> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		Array.Add(JsonData[i]->AsObject());
	}
	return Array;
}

/*
void FJsonArrayBP::AddObjectElement(const FString& ElementName, TSharedPtr<FJsonObject> Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetObjectField(ElementName, Value);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetObjectArray(const FString& ElementName, const TArray<TSharedPtr<FJsonObject>>& Array)
{
	JsonData.Empty();
	for (int i = 0; i < Array.Num(); i++) {
		AddObjectElement(ElementName, Array[i]);
	}
}

TArray<FJsonArrayBP> FJsonArrayBP::GetNestedArrays(const FString& ElementName)
{
	const int ElementCount = JsonData.Num();
	TArray<FJsonArrayBP> Array;
	Array.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		const TArray<TSharedPtr<FJsonValue>>* Temp;
		if (JsonData[i]->AsObject()->TryGetArrayField(ElementName, Temp)) {
			Array.Add(FJsonArrayBP(*Temp));
		}	
	}
	return Array;
}

void FJsonArrayBP::AddNestedArray(const FString& ElementName, FJsonArrayBP Value)
{
	TSharedPtr<FJsonObject> Element = MakeShareable(new FJsonObject());
	Element->SetArrayField(ElementName, Value.JsonData);
	JsonData.Add(MakeShareable(new FJsonValueObject(Element)));
}

void FJsonArrayBP::SetNestedArrays(const FString& ElementName, const TArray<FJsonArrayBP>& Array)
{
	JsonData.Empty();
	for (int i = 0; i < Array.Num(); i++) {
		AddNestedArray(ElementName, Array[i]);
	}
}
*/

FJsonObjectBP::FJsonObjectBP()
{
	MakeNewShareable();
}

FJsonObjectBP::FJsonObjectBP(const TSharedPtr<FJsonObject>& Object)
{
	JsonData = Object;
}

bool FJsonObjectBP::LoadJsonFile(const FString& Path, FJsonObjectBP& OutReadObject)
{
	if (!FPaths::FileExists(Path)) {
		iLog("file does not exist at " + Path);
		return false;
	}

	FString JsonString;
	TSharedPtr<FJsonObject> JsonObject;

	FFileHelper::LoadFileToString(JsonString, *Path);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	const FJsonSerializer::EFlags FlagOptions = FJsonSerializer::EFlags::None;
	const bool ReadSuccess = FJsonSerializer::Deserialize(JsonReader, JsonObject, FlagOptions);

	if (!ReadSuccess) {
		iLog("could not read file at " + Path);
		return false;
	}

	OutReadObject.JsonData = JsonObject;
	return true;
}

bool FJsonObjectBP::LoadJsonString(const FString& JsonString, FJsonObjectBP& OutReadObject)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	const FJsonSerializer::EFlags FlagOptions = FJsonSerializer::EFlags::None;
	const bool ReadSuccess = FJsonSerializer::Deserialize(JsonReader, JsonObject, FlagOptions);

	if (!ReadSuccess) {
		iLog("could not load json string...\n" + JsonString);
		return false;
	}

	OutReadObject.JsonData = JsonObject;
	return true;
}

TArray<FJsonObjectBP> FJsonObjectBP::ArrayFromObjects(const TArray<TSharedPtr<FJsonObject>>& Objects)
{
	const int ElementCount = Objects.Num();
	TArray<FJsonObjectBP> OutArray;
	OutArray.Reserve(ElementCount);
	for (int i = 0; i < ElementCount; i++) {
		OutArray.Add(FJsonObjectBP(Objects[i]));
	}
	return OutArray;
}

FJsonArrayBP FJsonObjectBP::JsonArrayFromObjects(const TArray<FJsonObjectBP>& Objects)
{
	FJsonArrayBP JsonArray;
	for (int i = 0; i < Objects.Num(); i++) {
		TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(Objects[i].JsonData));
		JsonArray.JsonData.Add(JsonValue);
	}
	return JsonArray;
}

FString FJsonObjectBP::ToString() const
{
	FString JsonString;
	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonData.ToSharedRef(), jsonWriter);
	return JsonString;
}

void FJsonObjectBP::SaveToFile(const FString& FilePath) const
{
	const FString JsonString = ToString();
	FFileHelper::SaveStringToFile(JsonString, *FilePath);
}

void FJsonObjectBP::MakeNewShareable()
{
	JsonData = MakeShareable(new FJsonObject());
}

bool FJsonObjectBP::HasField(const FString& FieldName) const
{
	return JsonData->HasField(FieldName);
}

bool FJsonObjectBP::GetBoolField(const FString& FieldName) const
{
	return JsonData->GetBoolField(FieldName);
}

void FJsonObjectBP::SetBoolField(const FString& FieldName, bool Value)
{
	JsonData->SetBoolField(FieldName, Value);
}

bool FJsonObjectBP::TryGetBoolField(const FString& FieldName, bool& Ref, bool UseDefault, bool DefaultValue) const
{
	if (!JsonData->TryGetBoolField(FieldName, Ref)) {
		if (UseDefault)
			Ref = DefaultValue;
		return false;
	}
	return true;
}

int FJsonObjectBP::GetIntegerField(const FString& FieldName) const
{
	return JsonData->GetIntegerField(FieldName);
}

void FJsonObjectBP::SetIntegerField(const FString& FieldName, int Value)
{
	JsonData->SetNumberField(FieldName, Value);
}

bool FJsonObjectBP::TryGetIntegerField(const FString& FieldName, int& Ref, bool UseDefault, int DefaultValue) const
{
	if (!JsonData->TryGetNumberField(FieldName, Ref)) {
		if (UseDefault)
			Ref = DefaultValue;
		return false;
	}
	return true;
}

int64 FJsonObjectBP::GetBigIntegerField(const FString& FieldName) const
{
	return JsonData->GetNumberField(FieldName);
}

void FJsonObjectBP::SetBigIntegerField(const FString& FieldName, int64 Value)
{
	JsonData->SetNumberField(FieldName, Value);
}

bool FJsonObjectBP::TryGetBigIntegerField(const FString& FieldName, int64& Ref, bool UseDefault, int64 DefaultValue) const
{
	if (!JsonData->TryGetNumberField(FieldName, Ref)) {
		if (UseDefault)
			Ref = DefaultValue;
		return false;
	}
	return true;
}

float FJsonObjectBP::GetFloatField(const FString& FieldName) const
{
	return JsonData->GetNumberField(FieldName);
}

void FJsonObjectBP::SetFloatField(const FString& FieldName, float Value)
{
	JsonData->SetNumberField(FieldName, Value);
}

bool FJsonObjectBP::TryGetFloatField(const FString& FieldName, float& Ref, bool UseDefault, float DefaultValue) const
{
	if (!JsonData->TryGetNumberField(FieldName, Ref)) {
		if (UseDefault)
			Ref = DefaultValue;
		return false;
	}
	return true;
}

FString FJsonObjectBP::GetStringField(const FString& FieldName) const
{
	return JsonData->GetStringField(FieldName);
}

void FJsonObjectBP::SetStringField(const FString& FieldName, const FString& Value)
{
	JsonData->SetStringField(FieldName, Value);
}

bool FJsonObjectBP::TryGetStringField(const FString& FieldName, FString& Ref, bool UseDefault, const FString& DefaultValue) const
{
	if (!JsonData->TryGetStringField(FieldName, Ref)) {
		if (UseDefault)
			Ref = DefaultValue;
		return false;
	}
	return true;
}

FJsonObjectBP FJsonObjectBP::GetObjectField(const FString& FieldName) const
{
	return FJsonObjectBP(JsonData->GetObjectField(FieldName));
}

void FJsonObjectBP::SetObjectField(const FString& FieldName, const FJsonObjectBP& Value)
{
	JsonData->SetObjectField(FieldName, Value.JsonData);
}

bool FJsonObjectBP::TryGetObjectField(const FString& FieldName, FJsonObjectBP& Ref) const
{
	const TSharedPtr<FJsonObject>* SubobjectJson;

	if (!JsonData->TryGetObjectField(FieldName, SubobjectJson)) {
		return false;
	}

	Ref.JsonData = *SubobjectJson;
	return true;
}

FJsonArrayBP FJsonObjectBP::GetArrayField(const FString& FieldName) const
{
	return FJsonArrayBP::FromArray(JsonData->GetArrayField(FieldName));
}

void FJsonObjectBP::SetArrayField(const FString& FieldName, const FJsonArrayBP& Value)
{
	JsonData->SetArrayField(FieldName, Value.JsonData);
}

bool FJsonObjectBP::TryGetArrayField(const FString& FieldName, FJsonArrayBP& Ref) const
{
	const TArray<TSharedPtr<FJsonValue>>* JsonArrayData;

	if (!JsonData->TryGetArrayField(FieldName, JsonArrayData)) {
		return false;
	}

	Ref.JsonData = *JsonArrayData;
	return true;
}

bool UJsonObjectLibrary::LoadJsonFile(const FString& FilePath, FJsonObjectBP& OutReadObject)
{
	return FJsonObjectBP::LoadJsonFile(FilePath, OutReadObject);
}

bool UJsonObjectLibrary::LoadJsonString(const FString& JsonString, FJsonObjectBP& OutReadObject)
{
	return FJsonObjectBP::LoadJsonString(JsonString, OutReadObject);
}

void UJsonObjectLibrary::SaveJsonToFile(const FString& FilePath, const FJsonObjectBP& JsonFile)
{
	JsonFile.SaveToFile(FilePath);
}

bool UJsonObjectLibrary::HasField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.HasField(FieldName);
}

bool UJsonObjectLibrary::GetBoolField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetBoolField(FieldName);
}

void UJsonObjectLibrary::SetBoolField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, bool Value)
{
	JsonObject.SetBoolField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetBoolField(const FJsonObjectBP& JsonObject, const FString& FieldName, bool& OutValue)
{
	return JsonObject.TryGetBoolField(FieldName, OutValue, false);
}

bool UJsonObjectLibrary::TryCopyBoolField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) bool& Ref, bool UseDefault, bool DefaultValue)
{
	return JsonObject.TryGetBoolField(FieldName, Ref, UseDefault, DefaultValue);
}

int UJsonObjectLibrary::GetIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetIntegerField(FieldName);
}

void UJsonObjectLibrary::SetIntegerField(UPARAM(ref)FJsonObjectBP& JsonObject, const FString& FieldName, int Value)
{
	JsonObject.SetIntegerField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, int& OutValue)
{
	return JsonObject.TryGetIntegerField(FieldName, OutValue, false);
}

bool UJsonObjectLibrary::TryCopyIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) int& Ref, bool UseDefault, int DefaultValue)
{
	return JsonObject.TryGetIntegerField(FieldName, Ref, UseDefault, DefaultValue);
}

int64 UJsonObjectLibrary::GetBigIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetBigIntegerField(FieldName);
}

void UJsonObjectLibrary::SetBigIntegerField(UPARAM(ref)FJsonObjectBP& JsonObject, const FString& FieldName, int64 Value)
{
	JsonObject.SetBigIntegerField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetBigIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, int64& OutValue)
{
	return JsonObject.TryGetBigIntegerField(FieldName, OutValue, false);
}

bool UJsonObjectLibrary::TryCopyBigIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref)int64& Ref, bool UseDefault, int64 DefaultValue)
{
	return JsonObject.TryGetBigIntegerField(FieldName, Ref, UseDefault, DefaultValue);
}

float UJsonObjectLibrary::GetFloatField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetFloatField(FieldName);
}

void UJsonObjectLibrary::SetFloatField(UPARAM(ref)FJsonObjectBP& JsonObject, const FString& FieldName, float Value)
{
	JsonObject.SetFloatField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetFloatField(const FJsonObjectBP& JsonObject, const FString& FieldName, float& OutValue)
{
	return JsonObject.TryGetFloatField(FieldName, OutValue, false);
}

bool UJsonObjectLibrary::TryCopyFloatField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) float& Ref, bool UseDefault, float DefaultValue)
{
	return JsonObject.TryGetFloatField(FieldName, Ref, UseDefault, DefaultValue);
}

FString UJsonObjectLibrary::GetStringField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetStringField(FieldName);
}

void UJsonObjectLibrary::SetStringField(UPARAM(ref)FJsonObjectBP& JsonObject, const FString& FieldName, const FString& Value)
{
	JsonObject.SetStringField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetStringField(const FJsonObjectBP& JsonObject, const FString& FieldName, FString& OutValue)
{
	return JsonObject.TryGetStringField(FieldName, OutValue, false);
}

bool UJsonObjectLibrary::TryCopyStringField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref)FString& Ref, bool UseDefault, const FString& DefaultValue)
{
	return JsonObject.TryGetStringField(FieldName, Ref, UseDefault, DefaultValue);
}

FJsonObjectBP UJsonObjectLibrary::GetObjectField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetObjectField(FieldName);
}

void UJsonObjectLibrary::SetObjectField(UPARAM(ref)FJsonObjectBP& JsonObject, const FString& FieldName, const FJsonObjectBP& Value)
{
	JsonObject.SetObjectField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetObjectField(const FJsonObjectBP& JsonObject, const FString& FieldName, FJsonObjectBP& OutValue)
{
	return JsonObject.TryGetObjectField(FieldName, OutValue);
}

FJsonArrayBP UJsonObjectLibrary::GetArrayField(const FJsonObjectBP& JsonObject, const FString& FieldName)
{
	return JsonObject.GetArrayField(FieldName);
}

void UJsonObjectLibrary::SetArrayField(UPARAM(ref)FJsonObjectBP& JsonObject, const FString& FieldName, const FJsonArrayBP& Value)
{
	JsonObject.SetArrayField(FieldName, Value);
}

bool UJsonObjectLibrary::TryGetArrayField(const FJsonObjectBP& JsonObject, const FString& FieldName, FJsonArrayBP& OutValue)
{
	return JsonObject.TryGetArrayField(FieldName, OutValue);
}
