// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h" //UButton 사용을 위해 포함

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true; //Error: InputMode:UIOnly 방지

	Level1 = Cast<UButton>(GetWidgetFromName(TEXT("btnLevel1")));
	if (Level1 != nullptr)
	{
		Level1->OnClicked.AddDynamic(this, &ULobbyWidget::OnOpenLevel1Clicked); //버튼에 델리게이트 바인딩
	}

	Level2 = Cast<UButton>(GetWidgetFromName(TEXT("btnLevel2")));
	if (Level2 != nullptr)
	{
		Level2->OnClicked.AddDynamic(this, &ULobbyWidget::OnOpenLevel2Clicked); //버튼에 델리게이트 바인딩
	}
}

void ULobbyWidget::OnOpenLevel1Clicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"));
}

void ULobbyWidget::OnOpenLevel2Clicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level2"));
}