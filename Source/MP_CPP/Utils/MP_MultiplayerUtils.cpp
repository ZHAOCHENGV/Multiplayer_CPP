// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_MultiplayerUtils.h"




void UMP_MultiplayerUtils::PrintLocalNetRole(AActor* Actor)
{
	if (Actor->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor： [%s] 有权限"), *Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor： [%s] 没有权限"), *Actor->GetName());
	}

	switch (Actor->GetLocalRole())
	{
	case ROLE_None:
		UE_LOG(LogTemp, Warning, TEXT("本地角色：无身份 (ROLE_None) - 通常在网络功能被禁用或对象尚未初始化时出现。"));
		break;
	case ROLE_SimulatedProxy:
		UE_LOG(LogTemp, Warning, TEXT("本地角色：模拟代理 (ROLE_SimulatedProxy) - 被动接收服务器数据的视觉表现，例如其他玩家的角色或网络同步的物体。"));
		break;
	case ROLE_AutonomousProxy:
		UE_LOG(LogTemp, Warning, TEXT("本地角色：自治代理 (ROLE_AutonomousProxy) - 可以预测性地处理本地输入的代理，专指被本地玩家控制器所拥有的角色。"));
		break;
	case ROLE_Authority:
		UE_LOG(LogTemp, Warning, TEXT("本地角色：权限方 (ROLE_Authority) - 拥有对该 Actor 状态的最终决定权，通常是服务器。"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("本地角色：未知"));
		break;
	}

}

void UMP_MultiplayerUtils::PrintRemoteNetRole(AActor* Actor)
{

	if (Actor->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor： [%s] 有权限"), *Actor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor： [%s] 没有权限"), *Actor->GetName());
	}
	
	switch (Actor->GetRemoteRole())
	{
	case ROLE_None:
		UE_LOG(LogTemp, Warning, TEXT("远程角色：无身份 (ROLE_None) - 通常在网络功能被禁用或对象尚未初始化时出现。"));
		break;
	case ROLE_SimulatedProxy:
		UE_LOG(LogTemp, Warning, TEXT("远程角色：模拟代理 (ROLE_SimulatedProxy) - 被动接收服务器数据的视觉表现，例如其他玩家的角色或网络同步的物体。"));
		break;
	case ROLE_AutonomousProxy:
		UE_LOG(LogTemp, Warning, TEXT("远程角色：自治代理 (ROLE_AutonomousProxy) - 可以预测性地处理本地输入的代理，专指被本地玩家控制器所拥有的角色。"));
		break;
	case ROLE_Authority:
		UE_LOG(LogTemp, Warning, TEXT("远程角色：权限方 (ROLE_Authority) - 拥有对该 Actor 状态的最终决定权，通常是服务器。"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("远程角色：未知"));
		break;
	}
}

/**
 * @brief 打印指定 Actor 的网络角色信息，包含本机和远端角色，并附带中文解释。
 * @param TargetActor 要查询其网络角色的 Actor。
 *
 * @par 功能说明
 * 这是一个非常实用的网络调试工具函数。它会获取一个 Actor，
 * 然后在 Output Log (输出日志) 中打印出关于这个 Actor 的一份详细的网络身份报告。
 * 这份报告能帮助开发者快速诊断和理解为什么一个 Actor 的行为
 * 在服务器和客户端上表现不同，是网络复制和 RPC (Remote Procedure Call) 调试的关键。
 *
 * @par 详细流程
 * 1.  **安全检查**: 首先检查传入的 `TargetActor` 指针是否有效，防止因空指针导致程序崩溃。
 * 2.  **创建辅助函数 (Lambda)**: 在函数内部定义一个名为 `RoleToString` 的临时辅助函数。
 *     这个函数的作用是将一个 `ENetRole` 枚举值转换为一个带有详细中文解释的、易于阅读的字符串。
 *     这样做可以避免在代码中重复写两遍相同的 `switch` 逻辑。
 * 3.  **获取角色字符串**: 调用 `RoleToString` 两次，分别获取 `TargetActor` 的本地角色和远程角色的描述字符串。
 * 4.  **获取上下文信息**: 获取执行这段代码时的环境信息，包括：
 *     - 当前世界是服务器、客户端还是单机模式。
 *     - 目标 Actor 的名字。
 *     - 目标 Actor 在当前机器上是否拥有权限 (`HasAuthority`)。
 * 5.  **格式化并输出**: 将所有收集到的信息用 `FString::Printf` 组合成一个格式优美的、多行的字符串。
 * 6.  **打印到日志**: 调用 `UE_LOG` 将这个最终的字符串一次性打印到输出日志中。
 *
 * @par 注意事项
 * - 这是一个静态工具函数，可以在任何地方调用，非常方便。
 * - `GPlayInEditorID` 是一个仅在编辑器中有效的全局变量，用于区分不同的 PIE (Play In Editor) 客户端窗口。在打包后的游戏中，这个值可能没有意义。
 */
void UMP_MultiplayerUtils::PrintNetRoleInfo(AActor* TargetActor)
{
	// 步骤 1: 增加健壮性检查，防止传入空指针导致崩溃。
	// IsValid() 是一个安全的检查，它会判断指针是否为 nullptr 以及其指向的对象是否有效。
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("PrintNetRoleInfo: 传入的 TargetActor 无效!"));
		return;
	}

	// 步骤 2: 获取并转换 Role 枚举值为带解释的字符串。
	// (为什么这么做): 为了避免重复编写两次几乎一样的 switch-case 逻辑（一次为 LocalRole，一次为 RemoteRole），
	// 我们在这里定义了一个“内联”的、临时的辅助函数。这用到了 C++11 的 Lambda 表达式特性。
	
	// --- Lambda 表达式详解 ---
	// auto RoleToString: `auto` 关键字让编译器自动推断 `RoleToString` 的类型，
	//                    它在这里的类型是一个 Lambda 函数。我们创建了一个名为 `RoleToString` 的临时函数变量。
	//
	// []: 这是捕获列表 (Capture Clause)。它定义了这个 Lambda 函数可以访问其外部作用域中的哪些变量。
	//     因为这个 Lambda 的功能是自包含的（它只需要传入的参数 Role），所以捕获列表为空。
	//
	// (ENetRole Role): 这是参数列表，和普通函数一样。这个 Lambda 接收一个名为 `Role` 的 `ENetRole` 类型的参数。
	//
	// -> FString: 这是返回类型声明。它明确地告诉编译器，这个 Lambda 函数将返回一个 `FString`。
	//
	// { ... }: 这是 Lambda 的函数体。包含了实际的执行逻辑。
	auto RoleToString = [](ENetRole Role) -> FString
	{
		switch (Role)
		{
			case ROLE_None:
				return TEXT("无身份 (ROLE_None) - 通常在网络功能被禁用或对象尚未初始化时出现。");
			case ROLE_SimulatedProxy:
				return TEXT("模拟代理 (ROLE_SimulatedProxy) - 被动接收服务器数据的视觉表现，例如其他玩家的角色或网络同步的物体。");
			case ROLE_AutonomousProxy:
				return TEXT("自治代理 (ROLE_AutonomousProxy) - 可以预测性地处理本地输入的代理，专指被本地玩家控制器所拥有的角色。");
			case ROLE_Authority:
				return TEXT("权限方 (ROLE_Authority) - 拥有对该 Actor 状态的最终决定权，通常是服务器。");
			default:
				return TEXT("未知角色");
		}
	};

	// 调用我们刚刚定义的 Lambda 函数，将 GetLocalRole() 的返回值传入，并将结果存储起来。
	const FString LocalRoleString = RoleToString(TargetActor->GetLocalRole());
	// 再次调用 Lambda 函数，处理 GetRemoteRole()。
	const FString RemoteRoleString = RoleToString(TargetActor->GetRemoteRole());

	// 步骤 3: 获取当前运行环境的上下文信息。
	FString WorldTypeString;
	// 使用 GetNetMode() 来判断网络模式，这是比旧的 IsServer() 更精确的方法。
	switch (TargetActor->GetWorld()->GetNetMode())
	{
		case NM_ListenServer:
			WorldTypeString = TEXT("监听服务器 (Listen Server)");
			break;
		case NM_DedicatedServer:
			WorldTypeString = TEXT("专用服务器 (Dedicated Server)");
			break;
		case NM_Client:
			// 在 PIE (Play In Editor) 模式下，客户端的 ID 从 2 开始。GPlayInEditorID > 0 ? GPlayInEditorID - 1 : 0 可以得到客户端编号。
			// 这在多客户端调试时非常有用，可以区分日志是哪个客户端打印的。
			WorldTypeString = FString::Printf(TEXT("客户端 (Client %d)"), GPlayInEditorID > 0 ? GPlayInEditorID - 1 : 0);
			break;
		case NM_Standalone:
			WorldTypeString = TEXT("单机模式 (Standalone)");
			break;
		default:
			WorldTypeString = TEXT("未知模式 (Unknown)");
			break;
	}

	const FString ActorName = TargetActor->GetName();// 获取 Actor 的名字。
	const FString AuthorityString = TargetActor->HasAuthority() ? TEXT("是 (Yes)") : TEXT("否 (No)");// 使用三元运算符快速判断并转换为字符串。


	// 步骤 4: 使用 FString::Printf 格式化并打印所有信息，提高日志可读性。
	// (为什么这么做): FString::Printf 可以像 C 语言的 printf 一样，将多个变量安全地格式化进一个字符串。
	// 连续使用 TEXT() 宏可以将多个字符串字面量连接成一个，让长字符串的书写更整洁。
	const FString LogMessage = FString::Printf(
		TEXT("\n===== 网络角色信息: [%s] =====\n")
		TEXT("   > 当前运行环境: %s\n")
		TEXT("   > 是否为权限方 (HasAuthority): %s\n")
		TEXT("   ------------------------------------\n")
		TEXT("   > 在 [本机] 的角色 (GetLocalRole):\n")
		TEXT("     - %s\n")
		TEXT("   > 在 [远端] 的角色 (GetRemoteRole):\n")
		TEXT("     - %s\n")
		TEXT("======================================="),
		*ActorName,
		*WorldTypeString,
		*AuthorityString,
		*LocalRoleString,
		*RemoteRoleString
	);
	// (为什么用*LogMessage): UE_LOG 需要一个 TCHAR* 类型的 C 风格字符串。
	// FString 类重载了 * 操作符，调用它会返回其内部存储的 TCHAR* 指针。
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
}

