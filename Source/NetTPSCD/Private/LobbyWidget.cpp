﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

//#define SWITCHER_INDEX_MENU 0
//#define SWITCHER_INDEX_CREATEROOM 1
//#define SWITCHER_INDEX_FINDROOM 2

const int SWITCHER_INDEX_MENU = 0;
const int SWITCHER_INDEX_CREATEROOM = 1;
const int SWITCHER_INDEX_FINDROOM = 2;

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = GetWorld()->GetGameInstance<UNetGameInstance>();

	btn_doCreateRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyClicked_doCreateRoom );

	btn_goCreateRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyGoCreateRoom );
	btn_goFindRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyGoFindRoom );
	btn_goMenuFromCreateRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyGoMenu );
	btn_goMenuFromFindRoom->OnClicked.AddDynamic( this , &ULobbyWidget::OnMyGoMenu );


	slider_maxPlayer->OnValueChanged.AddDynamic( this , &ULobbyWidget::OnMyValueChage_maxPlayer );

	text_maxPlayer->SetText( FText::AsNumber( slider_maxPlayer->GetValue() ) );
	slider_maxPlayer->SetMinValue( 2 );
	slider_maxPlayer->SetMaxValue( 10 );
	slider_maxPlayer->SetStepSize( 1 );
	slider_maxPlayer->MouseUsesStep = true;
}

void ULobbyWidget::OnMyValueChage_maxPlayer(float value)
{
	text_maxPlayer->SetText( FText::AsNumber( value ) );
}

void ULobbyWidget::OnMyClicked_doCreateRoom()
{
	// 만약 방이름이 비어있으면 함수를 종료하고싶다.
	FString roomName = edit_roomName->GetText().ToString();
	
	if (roomName.TrimStartAndEnd().IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("방 이름을 안 적었어요!!"));
		return;
	}

	TArray<FString> badWords;
	badWords.Add( TEXT( "바보" ) );
	badWords.Add( TEXT( "똥개" ) );
	for (int i = 0; i < badWords.Num(); i++)
	{
		if (roomName.Contains( badWords[i] ))
		{
			UE_LOG( LogTemp , Warning , TEXT( "방 이름이 적절하지 않습니다." ) );
			return;
		}
	}


	// UNetGameInstance::CreateRoom 를 호출하고싶다.
	int32 maxPlayer = slider_maxPlayer->GetValue();
	gi->CreateRoom( maxPlayer , roomName );
}

void ULobbyWidget::SwitchPanel(int32 index)
{
	widgetSwitcherLobby->SetActiveWidgetIndex( index );
}

void ULobbyWidget::OnMyGoMenu()
{
	SwitchPanel( SWITCHER_INDEX_MENU );
}

void ULobbyWidget::OnMyGoCreateRoom()
{
	SwitchPanel( SWITCHER_INDEX_CREATEROOM );
}

void ULobbyWidget::OnMyGoFindRoom()
{
	SwitchPanel( SWITCHER_INDEX_FINDROOM );
}
