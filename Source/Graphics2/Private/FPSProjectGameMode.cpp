// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectGameMode.h"
#include "Kismet/GameplayStatics.h"

void AFPSProjectGameMode::StartPlay()
{
    Super::StartPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController) 
    {
       //Resetting to Game Only Input
        FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false; //Hides the mouse cursor
    }

    check(GEngine != nullptr);

    // Display a debug message for five seconds. 
    // The -1 "Key" value argument prevents the message from being updated or refreshed.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameModeBase! This is Joshua")); //First way of debugging!

	//2. To print to console
    UE_LOG(LogTemp, Warning, TEXT("Hello World, this is FPSGameModeBase!This is Joshua")); 

	//3. Printing values
    int Testnumber = 10;
	FString Name("TestName");
    bool BoolTest = true;
	UE_LOG(LogTemp, Warning, TEXT("Testnumber: %i, Name: %s, BoolTest: %d"), Testnumber, *Name, BoolTest); //The * is used to dereference the string.

    //4. Modern way of debugging values
	UE_LOGFMT(LogTemp, Warning, "TestNumber: {0}, Name: {1}, BoolTest: {2}", Testnumber, Name, BoolTest); //can only use because of the structured log include in header file.
}
