// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BehaviorGeneratorFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EAttributeType
{
	Trait,
	Possession,
	Location,
	Object
};

UENUM(BlueprintType)
enum class ETarget
{
	None,
	Target1,
	Target2
};

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY()

	UPROPERTY()
	EAttributeType m_Type;
	UPROPERTY()
	bool m_IsNegated = false;
	UPROPERTY(BlueprintReadWrite)
	FString m_Value;
};

USTRUCT(BlueprintType)
struct FRequirement : public FAttribute
{
	GENERATED_BODY()

	UPROPERTY()
	ETarget m_Target;
};

UENUM(BlueprintType)
enum class EOperation
{
	Add,
	Remove,
	Move
};

USTRUCT(BlueprintType)
struct FConsequence
{
	GENERATED_BODY()

	UPROPERTY()
	EOperation m_Operation;
	UPROPERTY()
	EAttributeType m_Type;
	UPROPERTY()
	ETarget m_Target;
	UPROPERTY()
	FString m_Value;
};

USTRUCT(BlueprintType)
struct FAgentState
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString m_Name;
	UPROPERTY(BlueprintReadWrite)
	TArray<FAttribute> m_Attributes;
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	UPROPERTY()
	EAttributeType m_Type;
	UPROPERTY(BlueprintReadWrite)
	FString m_Value;
	UPROPERTY()
	TArray<FRequirement> m_Requirements;
};

USTRUCT(BlueprintType)
struct FAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString m_Name;
	UPROPERTY()
	TArray<FRequirement> m_Requirements;
	UPROPERTY()
	TArray<FConsequence> m_Consequences;
};

/**
 * 
 */
UCLASS()
class EMERGENTBEHAVIOUR_API UBehaviorGeneratorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static TArray<FAction> ParseActions(const UDataTable* ActionsDT);

	UFUNCTION(BlueprintCallable)
	static TArray<FAgentState> ParseAgents(const UDataTable* AgentsDT);

	UFUNCTION(BlueprintCallable)
	static TArray<FItem> ParseItems(const UDataTable* ItemsDT);

	UFUNCTION(BlueprintCallable)
	static TArray<FAction>& AddItemInteractionActions(const TArray<FItem> Items, UPARAM(ref) TArray<FAction>& AvailableActions);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FAgentState>& PrintGeneratedStory(UPARAM(ref) TArray<FAction>& AvailableActions, UPARAM(ref) TArray<FAgentState>& AvailableAgents, FString& StoryLine);

private:

	static FAction ParseLineIntoAction(TArray<FString> Line);

	static FAgentState ParseLineIntoAgent(TArray<FString> Line);

	static FItem ParseLineIntoItem(TArray<FString> Line);

	static bool AssignRoles(TArray<FAgentState>& AvailableAgents, const TArray<FRequirement>& Requirements, TArray<FAgentState>& OutSelectedAgents);

	static void ApplyConsequences(TArray<FAgentState>& AvailableAgents, const TArray<FConsequence>& Consequences, const TArray<FAgentState>& SelectedAgents);

	static bool DoesAgentMeetRequirement(const FAgentState& Agent, const FRequirement& Requirements);

	static FString GenerateTextForAction(const TArray<FAgentState>& Agents, FString ActionName);

	static FString GenerateTextForConsequences(const TArray<FConsequence>& Consequences);

	static void ShuffleActions(TArray<FAction>& Actions);

	static void ShuffleAgents(TArray<FAgentState>& Agents);
};
