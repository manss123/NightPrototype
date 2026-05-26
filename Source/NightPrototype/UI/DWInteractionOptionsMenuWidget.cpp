// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInteractionOptionsMenuWidget.h"

void UDWInteractionOptionsMenuWidget::NotifyOptionSelected(FDWInteractionOption Option)
{
	OnOptionSelectedNative.Broadcast(Option);
}
