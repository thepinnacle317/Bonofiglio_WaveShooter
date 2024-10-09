// Copyright SOUNDFX STUDIO © 2023

#include "MyUI.h"

void UMyUI::ErrorMessages(int32 n)
{
	if (!txt_errors)
		return;

	switch (n)
	{
	case 0:
		txt_errors->SetText(FText::FromString("PLEASE ADD SFX COMPONENT\nTO THE WEAPON"));
		break;
	case 99:
	default:
		// DUMMY
		break;
	}

	PlayAnimation(anim_TxtErrors);
}

void UMyUI::UI_Init()
{
	for (UBorder* wpn : weap)
		weapons.Add(wpn);

	weapons.Add(weap_1);
	weapons.Add(weap_2);
	weapons.Add(weap_3);
	weapons.Add(weap_4);
	weapons.Add(weap_5);
	weapons.Add(weap_6);
	weapons.Add(weap_7);
	weapons.Add(weap_8);
	weapons.Add(weap_9);
	weapons.Add(weap_10);
	weapons.Add(weap_11);
	weapons.Add(weap_12);
	weapons.Add(weap_13);
	weapons.Add(weap_14);
	weapons.Add(weap_15);
	weapons.Add(weap_16);
	weapons.Add(weap_17);
	weapons.Add(weap_18);
	weapons.Add(weap_19);
	weapons.Add(weap_20);
	weapons.Add(weap_21);
	weapons.Add(weap_22);
	weapons.Add(weap_23);
	weapons.Add(weap_24);
	weapons.Add(weap_25);
	weapons.Add(weap_26);
	weapons.Add(weap_27);
	weapons.Add(weap_28);
	weapons.Add(weap_29);
	weapons.Add(weap_30);
	weapons.Add(weap_31);
	weapons.Add(weap_32);
	weapons.Add(weap_33);
	UI_SwitchWeapon(0);
	UI_Fire(false);
	UI_FireMode(true);
}

void UMyUI::UI_SwitchWeapon(int wpnNumber)
{
	for (UBorder* wpn : weapons)
	{
		SetWpnOnOff(wpn, false);
	}
	if (weapons.Num() >= wpnNumber + 1)
		SetWpnOnOff(weapons[wpnNumber], true);
}

void UMyUI::UI_Fire(bool bOn)
{
	if (bOn)
		PlayAnimation(anim_Fire_Pressed);
	SetWpnOnOff(action_1, bOn);
}

void UMyUI::UI_FireMode(bool bAuto)
{
	SetWpnOnOff(action_2, bAuto);
}

void UMyUI::UI_Suppressor(bool bOn)
{
	SetWpnOnOff(action_3, bOn);
}

void UMyUI::UI_FireMode_Anim()
{
	PlayAnimation(anim_Firemode_Pressed);
}

void UMyUI::UI_Suppressor_Anim()
{
	PlayAnimation(anim_Suppressor_Pressed);
}

// Switch UI Buttons On/Off
void UMyUI::SetWpnOnOff(UBorder* wpn, bool on /*= true*/)
{
	if (!wpn)
		return;

	FLinearColor setColor = on ? txt_on_color : txt_off_color;
	wpn->SetContentColorAndOpacity(setColor);
	setColor = on ? txt_on_color : brush_off_color;
	wpn->SetBrushColor(setColor);
}