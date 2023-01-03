// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintJsonObject.generated.h"

USTRUCT(BlueprintType)
struct FJsonArrayBP 
{
	GENERATED_BODY()

	TArray<TSharedPtr<FJsonValue>> JsonData;

public:

	FJsonArrayBP();

	FJsonArrayBP(const TArray<TSharedPtr<FJsonValue>>& Array);

	static FJsonArrayBP FromArray(const TArray<TSharedPtr<FJsonValue>>& Array);

	/* Number of elements contained. */
	int Num();

	/* Convert json value array to boolean array given the name of the objects held within. */
	TArray<bool> GetBoolArray(const FString& ElementName) const;

	/* Add a singular boolean value to the array. Keep element name the same as the rest in the array. */
	void AddBoolElement(const FString& ElementName, bool Value);

	/* Empties current held json data. */
	void SetBoolArray(const FString& ElementName, const TArray<bool>& Array);

	/* Convert json value array to boolean array given the name of the objects held within. */
	TArray<int> GetIntegerArray(const FString& ElementName) const;

	/* Add a singular integer value to the array. Keep element name the same as the rest in the array. */
	void AddIntegerElement(const FString& ElementName, int Value);

	/* Empties current held json data. */
	void SetIntegerArray(const FString& ElementName, const TArray<int>& Array);

	/* Convert json value array to boolean array given the name of the objects held within. */
	TArray<int64> GetBigIntegerArray(const FString& ElementName) const;

	/* Add a singular 64 bit integer value to the array. Keep element name the same as the rest in the array. */
	void AddBigIntegerElement(const FString& ElementName, int64 Value);

	/* Empties current held json data. */
	void SetBigIntegerArray(const FString& ElementName, const TArray<int64>& Array);

	/* Convert json value array to boolean array given the name of the objects held within. */
	TArray<float> GetFloatArray(const FString& ElementName) const;

	/* Add a singular float value to the array. Keep element name the same as the rest in the array. */
	void AddFloatElement(const FString& ElementName, float Value);

	/* Empties current held json data. */
	void SetFloatArray(const FString& ElementName, const TArray<float>& Array);

	/* Convert json value array to boolean array given the name of the objects held within. */
	TArray<FString> GetStringArray(const FString& ElementName) const;

	/* Add a singular string value to the array. Keep element name the same as the rest in the array. */
	void AddStringElement(const FString& ElementName, const FString& Value);

	/* Empties current held json data. */
	void SetStringArray(const FString& ElementName, const TArray<FString>& Array);

	/* Get as the raw objects held within the array. Use FJsonObjectBP::ArrayFromObjects() for conversion. */
	TArray<TSharedPtr<FJsonObject>> GetObjectArray() const;

	/* Add a singular boolean value to the array. Keep element name the same as the rest in the array. */
	//void AddObjectElement(const FString& ElementName, TSharedPtr<FJsonObject> Value);

	/* Empties current held json data. */
	//void SetObjectArray(const FString& ElementName, const TArray<TSharedPtr<FJsonObject>>& Array);

	/* Convert json value array to boolean array given the name of the objects held within. */
	//TArray<FJsonArrayBP> GetNestedArrays(const FString& ElementName);

	/* Add a singular boolean value to the array. Keep element name the same as the rest in the array. */
	//void AddNestedArray(const FString& ElementName, FJsonArrayBP Value);

	/* Empties current held json data. */
	//void SetNestedArrays(const FString& ElementName, const TArray<FJsonArrayBP>& Array);

};

USTRUCT(BlueprintType)
struct FJsonObjectBP 
{
	GENERATED_BODY()

	TSharedPtr<FJsonObject> JsonData;

public:

	FJsonObjectBP();

	FJsonObjectBP(const TSharedPtr<FJsonObject>& Object);

	/* Loads a json file. Check bValid from return value to see if the object is valid.  */
	static bool LoadJsonFile(const FString& Path, FJsonObjectBP& OutReadObject);

	/* Loads a json file from a string. Check bValid from return value to see if the object is valid. */
	static bool LoadJsonString(const FString& JsonString, FJsonObjectBP& OutReadObject);

	/**/
	static TArray<FJsonObjectBP> ArrayFromObjects(const TArray<TSharedPtr<FJsonObject>>& Objects);

	/**/
	static FJsonArrayBP JsonArrayFromObjects(const TArray<FJsonObjectBP>& Objects);

	/* Serializes to json string. */
	FString ToString() const;

	/**/
	void SaveToFile(const FString& FilePath) const;

	/* Resets shared pointer to json data currently held, potentially deleting it. */
	void MakeNewShareable();

	/**/
	bool HasField(const FString& FieldName) const;

	/**/
	bool GetBoolField(const FString& FieldName) const;

	/**/
	void SetBoolField(const FString& FieldName, bool Value);

	/**/
	bool TryGetBoolField(const FString& FieldName, bool& Ref, bool UseDefault = true, bool DefaultValue = false) const;

	/**/
	int GetIntegerField(const FString& FieldName) const;

	/**/
	void SetIntegerField(const FString& FieldName, int Value);

	/**/
	bool TryGetIntegerField(const FString& FieldName, int& Ref, bool UseDefault = true, int DefaultValue = 0) const;

	/**/
	int64 GetBigIntegerField(const FString& FieldName) const;

	/**/
	void SetBigIntegerField(const FString& FieldName, int64 Value);

	/**/
	bool TryGetBigIntegerField(const FString& FieldName, int64& Ref, bool UseDefault = true, int64 DefaultValue = 0) const;

	/**/
	float GetFloatField(const FString& FieldName) const;

	/**/
	void SetFloatField(const FString& FieldName, float Value);

	/**/
	bool TryGetFloatField(const FString& FieldName, float& Ref, bool UseDefault = true, float DefaultValue = 0.f) const;

	/**/
	FString GetStringField(const FString& FieldName) const;

	/**/
	void SetStringField(const FString& FieldName, const FString& Value);

	/**/
	bool TryGetStringField(const FString& FieldName, FString& Ref, bool UseDefault = true, const FString& DefaultValue = "") const;

	/**/
	FJsonObjectBP GetObjectField(const FString& FieldName) const;

	/**/
	void SetObjectField(const FString& FieldName, const FJsonObjectBP& Value);

	/**/
	bool TryGetObjectField(const FString& FieldName, FJsonObjectBP& Ref) const;

	/**/
	FJsonArrayBP GetArrayField(const FString& FieldName) const;

	/**/
	void SetArrayField(const FString& FieldName, const FJsonArrayBP& Value);

	/**/
	bool TryGetArrayField(const FString& FieldName, FJsonArrayBP& Ref) const;

};

UCLASS()
class IMMIE_API UJsonObjectLibrary : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		/* Load json from a file on the local machine. */
		static bool LoadJsonFile(const FString& FilePath, FJsonObjectBP& OutReadObject);

	UFUNCTION(BlueprintCallable)
		/* Load json from a provided string in json format. */
		static bool LoadJsonString(const FString& JsonString, FJsonObjectBP& OutReadObject);

	UFUNCTION(BlueprintCallable)
		/* Save json to a file on the local machine at the specified path. NOTE: Need to include the file name and extension .json in the path. */
		static void SaveJsonToFile(const FString& FilePath, const FJsonObjectBP& JsonFile);

	UFUNCTION(BlueprintPure)
		/* Whether the json object has a specified field of any type. */
		static bool HasField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintPure)
		/**/
		static bool GetBoolField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetBoolField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, bool Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json. 
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetBoolField(const FJsonObjectBP& JsonObject, const FString& FieldName, bool& OutValue);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Return Value pin correponds to whether the field was successfully retrieved.
		The ref is a reference to the variable to set the, if found, value to. */
		static bool TryCopyBoolField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) bool& Ref, bool UseDefault = true, bool DefaultValue = false);

	UFUNCTION(BlueprintPure)
		/**/
		static int GetIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetIntegerField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, int Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json. 
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, int& OutValue);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Return Value pin correponds to whether the field was successfully retrieved.
		The ref is a reference to the variable to set the, if found, value to. */
		static bool TryCopyIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) int& Ref, bool UseDefault = true, int DefaultValue = 0);

	UFUNCTION(BlueprintPure)
		/**/
		static int64 GetBigIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetBigIntegerField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, int64 Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json.
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetBigIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, int64& OutValue);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Return Value pin correponds to whether the field was successfully retrieved.
		The ref is a reference to the variable to set the, if found, value to. */
		static bool TryCopyBigIntegerField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) int64& Ref, bool UseDefault = true, int64 DefaultValue = 0);

	UFUNCTION(BlueprintPure)
		/**/
		static float GetFloatField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetFloatField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, float Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json.
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetFloatField(const FJsonObjectBP& JsonObject, const FString& FieldName, float& OutValue);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Return Value pin correponds to whether the field was successfully retrieved.
		The ref is a reference to the variable to set the, if found, value to. */
		static bool TryCopyFloatField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) float& Ref, bool UseDefault = true, float DefaultValue = 0.f);

	UFUNCTION(BlueprintPure)
		/**/
		static FString GetStringField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetStringField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, const FString& Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json.
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetStringField(const FJsonObjectBP& JsonObject, const FString& FieldName, FString& OutValue);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Return Value pin correponds to whether the field was successfully retrieved.
		The ref is a reference to the variable to set the, if found, value to. */
		static bool TryCopyStringField(const FJsonObjectBP& JsonObject, const FString& FieldName, UPARAM(ref) FString& Ref, bool UseDefault = true, const FString& DefaultValue = "");

	UFUNCTION(BlueprintPure)
		/**/
		static FJsonObjectBP GetObjectField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetObjectField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, const FJsonObjectBP& Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json.
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetObjectField(const FJsonObjectBP& JsonObject, const FString& FieldName, FJsonObjectBP& OutValue);

	UFUNCTION(BlueprintPure)
		/**/
		static FJsonArrayBP GetArrayField(const FJsonObjectBP& JsonObject, const FString& FieldName);

	UFUNCTION(BlueprintCallable)
		/**/
		static void SetArrayField(UPARAM(ref) FJsonObjectBP& JsonObject, const FString& FieldName, const FJsonArrayBP& Value);

	UFUNCTION(BlueprintCallable)
		/* For blueprint, the Out Value pin corresponds to the actual read value from json.
		The Return Value pin corresponds to whether the field was successfully retrieved. */
		static bool TryGetArrayField(const FJsonObjectBP& JsonObject, const FString& FieldName, FJsonArrayBP& OutValue);

};
