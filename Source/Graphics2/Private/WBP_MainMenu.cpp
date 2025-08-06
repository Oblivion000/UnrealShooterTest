// Fill out your copyright notice in the Description page of Project Settings.


#include "WBP_MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UWBP_MainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(this, &UWBP_MainMenu::OnPlayButtonClicked);

    if (SettingsButton)
        SettingsButton->OnClicked.AddDynamic(this, &UWBP_MainMenu::OnSettingsButtonClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UWBP_MainMenu::OnQuitButtonClicked);
}

void UWBP_MainMenu::OnPlayButtonClicked()
{
    if (FadeOut)
    {
		UE_LOG(LogTemp, Warning, TEXT("Playing FadeOut Animation..."));
        PlayAnimation(FadeOut);

        //Delay level load until animatoin ends
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
            {
                UGameplayStatics::OpenLevel(this, "MainLevel"); // Replace with your level name
				UE_LOG(LogTemp, Warning, TEXT("Loading Main Level..."));
            }
            , FadeOut->GetEndTime(), false);
    }
    else 
    {
        UGameplayStatics::OpenLevel(this, "MainLevel"); // Replace with your level name
    }
}

void UWBP_MainMenu::OnSettingsButtonClicked()
{
    // Hide current menu, show settings menu (depends on your manager logic)
}

void UWBP_MainMenu::OnQuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
