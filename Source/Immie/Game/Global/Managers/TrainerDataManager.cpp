// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerDataManager.h"
#include "../../../Overworld/Trainer/TrainerDataObject.h"

void UTrainerDataManager::RegisterTrainers(UObject* Outer, TMap<FName, UTrainerDataObject*>* MapOut)
{
#define RegisterTrainerName(name) RegisterTrainer(Outer, name, MapOut)

	RegisterTrainerName("testTrainer");
}

void UTrainerDataManager::RegisterTrainer(UObject* Outer, FName TrainerName, TMap<FName, UTrainerDataObject*>* MapOut)
{
	UTrainerDataObject* DataObject = LoadTrainerData(Outer, TrainerName);
	MapOut->Add(TrainerName, DataObject);
}

UTrainerDataObject* UTrainerDataManager::GetTrainerDataObject(FName TrainerName)
{
	UTrainerDataObject** Found = Trainers.Find(TrainerName);
	if (Found) {
		return *Found;
	}

	iLog("Could not find trainer data object for trainer " + TrainerName.ToString(), LogVerbosity_Error);
	return nullptr;
}

void UTrainerDataManager::LoadDefaultGameData()
{
	Trainers.Empty();

	RegisterTrainers(this, &Trainers);
}

FString UTrainerDataManager::LoadTrainerJsonToString(FName TrainerName)
{
	static const FString TrainerDir = FPaths::Combine(FPaths::ProjectContentDir(), "Data/Trainers/");
	const FString JsonPath = TrainerDir + TrainerName.ToString() + ".json";
	if (!FPaths::FileExists(JsonPath)) {
		iLog("File does not exist at path: " + JsonPath);
		return FString();
	}

	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonPath);
	return JsonString;
}

TSubclassOf<UTrainerDataObject> UTrainerDataManager::GetTrainerDataObjectClass(FName TrainerName)
{
	const FString TrainerString = UStringUtils::ToUpperFirstLetter(TrainerName.ToString());
	const FString DataObjectClassReferenceString = UTrainerDataObject::GetTrainerBlueprintFolder() + TrainerString + "/BP_" + TrainerString + "DataObject.BP_" + TrainerString + "DataObject_C'";
	TSubclassOf<UTrainerDataObject> DataObjectClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *DataObjectClassReferenceString));
	if (!IsValid(DataObjectClass)) {
		iLog("Data object UClass for trainer " + TrainerName.ToString() + " is not valid.", LogVerbosity_Error);
		return nullptr;
	}
	return DataObjectClass;
}

UTrainerDataObject* UTrainerDataManager::LoadTrainerData(UObject* Outer, FName TrainerName)
{
	TSubclassOf<UTrainerDataObject> DataObjectClass = GetTrainerDataObjectClass(TrainerName);
	const FString JsonString = LoadTrainerJsonToString(TrainerName);
	FJsonObjectBP TrainerJson;
	if (!FJsonObjectBP::LoadJsonString(JsonString, TrainerJson)) {
		iLog("Unable to parse trainer json " + TrainerName.ToString(), LogVerbosity_Error);
		return nullptr;
	}

	UTrainerDataObject* DataObject = NewObject<UTrainerDataObject>(this, DataObjectClass);
	DataObject->LoadTrainerJsonData(TrainerJson);
	return DataObject;
}
