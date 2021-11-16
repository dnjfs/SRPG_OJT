// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h" //UButton 사용을 위해 포함

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Level1 = Cast<UButton>(GetWidgetFromName(TEXT("btnLevel1")));
	if (Level1 != nullptr)
	{
		Level1->OnClicked.AddDynamic(this, &ULobbyWidget::OnOpenLevel1Clicked);
	}
}

void ULobbyWidget::OnOpenLevel1Clicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"));
}