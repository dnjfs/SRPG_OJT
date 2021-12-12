// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultWidget.h"
#include "Components/TextBlock.h"

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameResultText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	FinalTurnText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTurn")));
}

void UResultWidget::SetResultMessage(bool bIsWin, int inTurnCount)
{
	if(bIsWin)
	{
		GameResultText->SetText(FText::FromString(TEXT("You Win!")));
	}
	else
	{
		GameResultText->SetText(FText::FromString(TEXT("You Lose..")));
	}

	FinalTurnText->SetText(FText::FromString(TEXT("Turn: ") + FString::FromInt(inTurnCount)));
}