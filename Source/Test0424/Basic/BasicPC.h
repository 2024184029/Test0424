// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPC.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class TEST0424_API ABasicPC : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TSoftObjectPtr<UInputMappingContext>> IMC_Basics;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	
};
