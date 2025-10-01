// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_LANMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"


/**
 * @brief 在此 UI Widget 初始化时被调用，用于设置输入模式和绑定按钮点击事件。
 *
 * @par 功能说明
 * 这是 UMG (Unreal Motion Graphics) 控件的初始化函数。它的核心职责是进行一次性的设置，
 * 确保当这个菜单显示时，玩家的输入焦点在 UI 上，并且可以看到鼠标光标，
 * 同时将“创建主机”和“加入游戏”按钮的点击事件与对应的处理函数关联起来。
 */
void UMP_LANMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 步骤 1/2: 设置输入模式为“仅 UI”。
	// (为什么这么做): FInputModeUIOnly 会让所有的键盘和鼠标输入都只作用于 UI 控件上，
	// 而不会影响到背景中的游戏世界（例如，防止玩家在点按钮时不小心开枪）。
	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	// 显示鼠标光标，以便玩家可以点击按钮。
	GetOwningPlayer()->SetShowMouseCursor(true);

	// 步骤 2/2: 动态绑定按钮的点击委托。
	// 当 Button_Host 被点击时，自动调用本类的 HostButtonClicked 函数。
	Button_Host->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	// 当 Button_Join 被点击时，自动调用本类的 JoinButtonClicked 函数。
	Button_Join->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
}

/**
 * @brief “创建主机”按钮的点击事件处理函数。
 *
 * @par 功能说明
 * 当玩家点击“创建主机”按钮后，此函数被执行。它会将输入模式切回“仅游戏”，
 * 并通过特殊的 URL 参数打开一个新的关卡，使其成为一个监听连接的网络主机。
 */
void UMP_LANMenu::HostButtonClicked()
{
	// 将输入模式切回“仅游戏”，隐藏鼠标光标，准备进入游戏状态。
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	// 步骤 2/2: 作为主机打开一个新关卡。
	// (为什么这么做): 这是创建网络主机的关键。
	// 第三个参数 `true` 表示这是一个 Listen Server (监听服务器)。
	// 第四个参数 `TEXT("?listen")` 是 URL 选项，它告诉引擎在加载完这个关卡后，
	// 开始监听来自其他客户端的网络连接请求。
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, HostingLevel, true, TEXT("listen"));
}



/**
 * @brief “加入游戏”按钮的点击事件处理函数。
 *
 * @par 功能说明
 * 当玩家点击“加入游戏”按钮后，此函数被执行。它会获取玩家在输入框中填写的 IP 地址，
 * 并尝试连接到该地址。
 */
void UMP_LANMenu::JoinButtonClicked()
{
	// 同样，将输入模式切回“仅游戏”。
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);

	// 步骤 2/2: 作为客户端连接到指定 IP 地址。
	// 从 UI 的文本输入框中获取文本，并转换为 FString。
	const FString Address = TextBox_IpAddress->GetText().ToString();
	// (为什么这么做): OpenLevel 不仅可以打开本地关卡，也可以通过传入一个 IP 地址来让客户端连接到该地址的主机。
	// FString 前面的 `*` 是一个重载的操作符，用于将 FString 转换为 TCHAR* (C风格字符串)，以符合 OpenLevel 函数的参数要求。
	UGameplayStatics::OpenLevel(this, *Address);
}
