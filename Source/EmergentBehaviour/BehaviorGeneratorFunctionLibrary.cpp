// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/KismetArrayLibrary.h"
#include "BehaviorGeneratorFunctionLibrary.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBehaviorGenerator, Log, All);
DEFINE_LOG_CATEGORY(LogBehaviorGenerator);

TArray<FAction> UBehaviorGeneratorFunctionLibrary::ParseActions(const UDataTable* ActionsDT)
{
	TArray<FAction> ParsedActions;

	const TCHAR* Terminators[] = { L"\r", L"\n" };
	const TCHAR* CSVDelimiters[] = { TEXT(",") };

	TArray<FString> CSVLines;
	FString CSVTable = ActionsDT->GetTableAsCSV();
	CSVTable = CSVTable.Replace(L"\\", L"");
	CSVTable = CSVTable.Replace(L"\"", L"");
	CSVTable.ParseIntoArray(CSVLines, Terminators, 2);

	TArray<FString> TempLine;
	for (int i = 1; i < CSVLines.Num(); i++)
	{
		TempLine.Empty();
		CSVLines[i].ParseIntoArray(TempLine, CSVDelimiters, 1);
		ParsedActions.Add(ParseLineIntoAction(TempLine));
	}

	return ParsedActions;
}

TArray<FAgentState> UBehaviorGeneratorFunctionLibrary::ParseAgents(const UDataTable* AgentsDT)
{
	TArray<FAgentState> ParsedAgents;

	const TCHAR* Terminators[] = { L"\r", L"\n" };
	const TCHAR* CSVDelimiters[] = { TEXT(",") };

	TArray<FString> CSVLines;
	FString CSVTable = AgentsDT->GetTableAsCSV();
	CSVTable = CSVTable.Replace(L"\\", L"");
	CSVTable = CSVTable.Replace(L"\"", L"");
	CSVTable.ParseIntoArray(CSVLines, Terminators, 2);

	TArray<FString> TempLine;
	for (int i = 1; i < CSVLines.Num(); i++)
	{
		TempLine.Empty();
		CSVLines[i].ParseIntoArray(TempLine, CSVDelimiters, 1);
		ParsedAgents.Add(ParseLineIntoAgent(TempLine));
	}

	return ParsedAgents;
}

TArray<FItem> UBehaviorGeneratorFunctionLibrary::ParseItems(const UDataTable* ItemsDT)
{
	TArray<FItem> ParsedItems;

	const TCHAR* Terminators[] = { L"\r", L"\n" };
	const TCHAR* CSVDelimiters[] = { TEXT(",") };

	TArray<FString> CSVLines;
	FString CSVTable = ItemsDT->GetTableAsCSV();
	CSVTable = CSVTable.Replace(L"\\", L"");
	CSVTable = CSVTable.Replace(L"\"", L"");
	CSVTable.ParseIntoArray(CSVLines, Terminators, 2);

	TArray<FString> TempLine;
	for (int i = 1; i < CSVLines.Num(); i++)
	{
		TempLine.Empty();
		CSVLines[i].ParseIntoArray(TempLine, CSVDelimiters, 1);
		ParsedItems.Add(ParseLineIntoItem(TempLine));
	}

	return ParsedItems;
}

FAction UBehaviorGeneratorFunctionLibrary::ParseLineIntoAction(TArray<FString> Line)
{
	FAction Action;
	bool bIsRequirement = true;

	for (FString Value : Line)
	{
		if (Value.StartsWith(L"="))
		{
			bIsRequirement = false;
			continue;
		}

		if (Value.StartsWith(L"Name_"))
		{
			Value.RemoveAt(0, 5);
			Value = Value.Replace(L"_", L" ");
			Action.m_Name = Value;
			continue;
		}

		if (bIsRequirement)
		{
			FRequirement Requirement;

			if (Value.StartsWith(L"T"))
			{
				Requirement.m_Type = EAttributeType::Trait;
			}
			else if (Value.StartsWith(L"P"))
			{
				Requirement.m_Type = EAttributeType::Possession;
			}

			Value.RemoveAt(0);

			if (Value.StartsWith(L"L_"))
			{
				Requirement.m_Type = EAttributeType::Location;
				Value.RemoveAt(0, 2);
			}

			Requirement.m_Target = ETarget::Target1;

			if (Value.StartsWith(L"1"))
			{
				Requirement.m_Target = ETarget::Target1;
				Value.RemoveAt(0);
			}
			else if (Value.StartsWith(L"2"))
			{
				Requirement.m_Target = ETarget::Target2;
				Value.RemoveAt(0);
			}

			Value.RemoveAt(0);

			if (Value.StartsWith(L"N_"))
			{
				Requirement.m_IsNegated = true;
				Value.RemoveAt(0, 2);
			}

			Requirement.m_Value = Value;

			Action.m_Requirements.Add(Requirement);
			
			continue;
		}

		if (!bIsRequirement)
		{
			FConsequence Consequence;

			if (Value.StartsWith(L"A"))
			{
				Consequence.m_Operation = EOperation::Add;
				Value.RemoveAt(0, 2);
			}
			else if (Value.StartsWith(L"R"))
			{
				Consequence.m_Operation = EOperation::Remove;
				Value.RemoveAt(0, 2);
			}

			if (Value.StartsWith(L"T"))
			{
				Consequence.m_Type = EAttributeType::Trait;
			}
			else if (Value.StartsWith(L"P"))
			{
				Consequence.m_Type = EAttributeType::Possession;
			}
			else if (Value.StartsWith(L"L"))
			{
				Consequence.m_Type = EAttributeType::Location;
			}

			Value.RemoveAt(0);

			Consequence.m_Target = ETarget::Target1;

			if (Value.StartsWith(L"1"))
			{
				Consequence.m_Target = ETarget::Target1;
				Value.RemoveAt(0);
			}
			else if (Value.StartsWith(L"2"))
			{
				Consequence.m_Target = ETarget::Target2;
				Value.RemoveAt(0);
			}

			Value.RemoveAt(0);

			Consequence.m_Value = Value;

			Action.m_Consequences.Add(Consequence);

			continue;
		}
	}

	return Action;
}

FAgentState UBehaviorGeneratorFunctionLibrary::ParseLineIntoAgent(TArray<FString> Line)
{
	FAgentState Agent;

	for (FString Value : Line)
	{
		if (Value.StartsWith(L"Name_"))
		{
			Agent.m_Name = Value.Mid(5);
			continue;
		}

		FAttribute Attribute;

		if (Value.StartsWith(L"T"))
		{
			Attribute.m_Type = EAttributeType::Trait;
		}
		else if (Value.StartsWith(L"P"))
		{
			Attribute.m_Type = EAttributeType::Possession;
		}
		else if (Value.StartsWith(L"L"))
		{
			Attribute.m_Type = EAttributeType::Location;
		}

		Value.RemoveAt(0, 2);

		if (Value.StartsWith(L"N_"))
		{
			Attribute.m_IsNegated = true;
			Value.RemoveAt(0, 2);
		}

		Attribute.m_Value = Value;

		Agent.m_Attributes.Add(Attribute);
	}

	return Agent;
}

FItem UBehaviorGeneratorFunctionLibrary::ParseLineIntoItem(TArray<FString> Line)
{
	FItem Item;

	for (FString Value : Line)
	{
		if (Value.StartsWith(L"Name_"))
		{
			Item.m_Value = Value.Mid(5);

			Value.RemoveAt(0, 5);

			if (Value.StartsWith(L"O"))
			{
				Item.m_Type = EAttributeType::Object;
			}
			else if (Value.StartsWith(L"L"))
			{
				Item.m_Type = EAttributeType::Location;
			}

			Value.RemoveAt(0, 2);

			Item.m_Value = Value;
		}
		else
		{
			FRequirement Requirement;

			if (Value.StartsWith(L"T"))
			{
				Requirement.m_Type = EAttributeType::Trait;
			}
			else if (Value.StartsWith(L"P"))
			{
				Requirement.m_Type = EAttributeType::Possession;
			}

			Value.RemoveAt(0);

			if (Value.StartsWith(L"L_"))
			{
				Requirement.m_Type = EAttributeType::Location;
				Value.RemoveAt(0, 2);
			}

			Requirement.m_Target = ETarget::Target1;

			if (Value.StartsWith(L"1"))
			{
				Requirement.m_Target = ETarget::Target1;
				Value.RemoveAt(0);
			}
			else if (Value.StartsWith(L"2"))
			{
				Requirement.m_Target = ETarget::Target2;
				Value.RemoveAt(0);
			}

			Value.RemoveAt(0);

			if (Value.StartsWith(L"N_"))
			{
				Requirement.m_IsNegated = true;
				Value.RemoveAt(0, 2);
			}

			Requirement.m_Value = Value;

			Item.m_Requirements.Add(Requirement);
		}
	}

	return Item;
}

TArray<FAction>& UBehaviorGeneratorFunctionLibrary::AddItemInteractionActions(const TArray<FItem> Items, UPARAM(ref) TArray<FAction>& AvailableActions)
{
	for (const FItem& Item : Items)
	{
		FAction Action;

		if (Item.m_Type == EAttributeType::Location)
		{
			Action.m_Name = "going to " + Item.m_Value;

			Action.m_Requirements = Item.m_Requirements;

			FRequirement Requirement;

			Requirement.m_IsNegated = true;
			Requirement.m_Type = EAttributeType::Location;
			Requirement.m_Target = ETarget::Target1;
			Requirement.m_Value = Item.m_Value;
			
			FConsequence Consequence;

			Consequence.m_Operation = EOperation::Move;
			Consequence.m_Type = EAttributeType::Location;
			Consequence.m_Target = ETarget::Target1;
			Consequence.m_Value = Item.m_Value;

			Action.m_Requirements.Add(Requirement);
			Action.m_Consequences.Add(Consequence);
		}
		else if (Item.m_Type == EAttributeType::Object)
		{
			Action.m_Name = "picking up %s", Item.m_Value;

			Action.m_Requirements = Item.m_Requirements;

			FRequirement Requirement;

			Requirement.m_IsNegated = true;
			Requirement.m_Type = EAttributeType::Possession;
			Requirement.m_Target = ETarget::Target1;
			Requirement.m_Value = Item.m_Value;

			FConsequence Consequence;

			Consequence.m_Operation = EOperation::Add;
			Consequence.m_Type = EAttributeType::Object;
			Consequence.m_Target = ETarget::Target1;
			Consequence.m_Value = Item.m_Value;

			Action.m_Requirements.Add(Requirement);
			Action.m_Consequences.Add(Consequence);
		}

		AvailableActions.Add(Action);
	}

	return AvailableActions;
}

TArray<FAgentState>& UBehaviorGeneratorFunctionLibrary::PrintGeneratedStory(TArray<FAction>& AvailableActions, TArray<FAgentState>& AvailableAgents, FString& StoryLine)
{
	TArray<FAgentState> SelectedAgents;

	ShuffleActions(AvailableActions);

	// ShuffleAgents(AvailableAgents);

	for (const FAction& Action : AvailableActions)
	{
		if (AssignRoles(AvailableAgents, Action.m_Requirements, SelectedAgents))
		{
			ApplyConsequences(AvailableAgents, Action.m_Consequences, SelectedAgents);

			StoryLine = FString::Printf(TEXT("%s: %s"), *GenerateTextForAction(SelectedAgents, Action.m_Name), *GenerateTextForConsequences(Action.m_Consequences));

			return AvailableAgents;
		}
		else
		{
			SelectedAgents.Empty();
		}
	}

	if (SelectedAgents.Num() <= 0)
	{
		StoryLine = FString::Printf(TEXT("No actions available."));
	}

	return AvailableAgents;
}

bool UBehaviorGeneratorFunctionLibrary::AssignRoles(TArray<FAgentState>& AvailableAgents, const TArray<FRequirement>& Requirements, TArray<FAgentState>& OutSelectedAgents)
{
	OutSelectedAgents.Empty();

	TArray<FRequirement> FirstAgentRequirements;
	TArray<FRequirement> SecondAgentRequirements;

	for (const FRequirement& Requirement : Requirements)
	{
		if (Requirement.m_Target == ETarget::Target1)
		{
			FirstAgentRequirements.Add(Requirement);
		}
		else if (Requirement.m_Target == ETarget::Target2)
		{
			SecondAgentRequirements.Add(Requirement);
		}
	}

	FAgentState SelectedFirstAgent;
	FAgentState SelectedSecondAgent;

	for (const FAgentState& Agent : AvailableAgents)
	{
		bool MeetsAllRequirements = true;
		
		for (const FRequirement& Requirement : FirstAgentRequirements)
		{
			if (!DoesAgentMeetRequirement(Agent, Requirement))
			{
				MeetsAllRequirements = false;
				break;
			}
		}

		if (!MeetsAllRequirements)
		{
			continue;
		}
		else
		{
			SelectedFirstAgent = Agent;
			OutSelectedAgents.Add(SelectedFirstAgent);
			break;
		}
	}

	if (SecondAgentRequirements.Num() > 0)
	{
		for (const FAgentState& Agent : AvailableAgents)
		{
			if (Agent.m_Name == SelectedFirstAgent.m_Name)
			{
				continue;
			}

			bool MeetsAllRequirements = true;

			for (const FRequirement& Requirement : SecondAgentRequirements)
			{
				if (!DoesAgentMeetRequirement(Agent, Requirement))
				{
					MeetsAllRequirements = false;
					break;
				}
			}

			if (!MeetsAllRequirements)
			{
				continue;
			}
			else
			{
				SelectedSecondAgent = Agent;
				OutSelectedAgents.Add(SelectedSecondAgent);
				break;
			}
		}
	}

	return SecondAgentRequirements.Num() > 0 ? OutSelectedAgents.Num() == 2 : OutSelectedAgents.Num() == 1;
}

void UBehaviorGeneratorFunctionLibrary::ApplyConsequences(TArray<FAgentState>& AvailableAgents, const TArray<FConsequence>& Consequences, const TArray<FAgentState>& SelectedAgents)
{
	for (const FConsequence& Consequence : Consequences)
	{
		for (int32 SelectedAgentIdx = 0; SelectedAgentIdx < SelectedAgents.Num(); SelectedAgentIdx++)
		{
			for (FAgentState& AvailableAgent : AvailableAgents)
			{
				if (SelectedAgents[SelectedAgentIdx].m_Name == AvailableAgent.m_Name)
				{
					if ((Consequence.m_Target == ETarget::Target1 && SelectedAgentIdx == 0)
						|| (Consequence.m_Target == ETarget::Target2 && SelectedAgentIdx == 1))
					{
						if (Consequence.m_Operation == EOperation::Add)
						{
							FAttribute AttributeToAdd;
							AttributeToAdd.m_Type = Consequence.m_Type;
							AttributeToAdd.m_Value = Consequence.m_Value;

							AvailableAgent.m_Attributes.Add(AttributeToAdd);
						}
						else if (Consequence.m_Operation == EOperation::Remove)
						{
							for (int i = AvailableAgent.m_Attributes.Num() - 1; i >= 0; i--)
							{
								if (AvailableAgent.m_Attributes[i].m_Type == Consequence.m_Type
									&& AvailableAgent.m_Attributes[i].m_Value == Consequence.m_Value)
								{
									AvailableAgent.m_Attributes.RemoveAt(i);
								}
							}
						}
						else if (Consequence.m_Operation == EOperation::Move)
						{
							for (FAttribute& Attribute : AvailableAgent.m_Attributes)
							{
								if (Attribute.m_Type == EAttributeType::Location)
								{
									Attribute.m_Value = Consequence.m_Value;
								}
							}
						}

						break;
					}
				}
			}
		}
	}
}

bool UBehaviorGeneratorFunctionLibrary::DoesAgentMeetRequirement(const FAgentState& Agent, const FRequirement& Requirement)
{
	bool MeetsRequirement = Requirement.m_IsNegated;

	for (const FAttribute& Attribute : Agent.m_Attributes)
	{
		if (Requirement.m_IsNegated)
		{
			if (Requirement.m_Value == Attribute.m_Value)
			{
				MeetsRequirement = false;

				break;
			}
		}
		else
		{
			if (Requirement.m_Value == Attribute.m_Value)
			{
				return true;
			}
		}
	}

	return MeetsRequirement;
}

FString UBehaviorGeneratorFunctionLibrary::GenerateTextForAction(const TArray<FAgentState>& Agents, FString ActionName)
{
	FString Text = Agents[0].m_Name;
	
	Text.Append(" is ");

	if (Agents.Num() > 1)
	{
		ActionName = ActionName.Replace(L"{T2}", *Agents[1].m_Name);
	}

	Text.Append(ActionName);

	return Text;
}

FString UBehaviorGeneratorFunctionLibrary::GenerateTextForConsequences(const TArray<FConsequence>& Consequences)
{
	FString Text;

	for (int i = 0; i < Consequences.Num(); i++)
	{
		if (i > 0)
		{
			Text.Append(" and ");
		}

		if (Consequences[i].m_Operation == EOperation::Add)
		{
			Text.Append("added ");
		}
		else if (Consequences[i].m_Operation == EOperation::Remove)
		{
			Text.Append("removed ");
		}
		else if (Consequences[i].m_Operation == EOperation::Move)
		{
			Text.Append("moved to ");
		}

		Text.Append(Consequences[i].m_Value);
	}

	return Text;
}

void UBehaviorGeneratorFunctionLibrary::ShuffleActions(TArray<FAction>& Actions)
{
	if (Actions.Num() > 0)
	{
		int32 LastIndex = Actions.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Actions.Swap(i, Index);
			}
		}
	}
}

void UBehaviorGeneratorFunctionLibrary::ShuffleAgents(TArray<FAgentState>& Agents)
{
	if (Agents.Num() > 0)
	{
		int32 LastIndex = Agents.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Agents.Swap(i, Index);
			}
		}
	}
}