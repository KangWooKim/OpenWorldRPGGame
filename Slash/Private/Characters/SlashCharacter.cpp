// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

// 생성자 함수
ASlashCharacter::ASlashCharacter()
{
	// 이 Actor는 매 틱마다 업데이트되어야 합니다.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러의 회전을 사용하지 않습니다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터의 이동 방향으로 회전하도록 설정합니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// 캐릭터의 메시의 충돌 설정을 설정합니다.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECCollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECCollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// 카메라와 카메라 붐을 설정합니다.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

}

// 매 틱마다 호출되는 함수
void ASlashCharacter::Tick(float DeltaTime)
{
	// 속성과 오버레이가 설정되어 있다면, 스태미나를 회복하고 오버레이에 표시합니다.
	if (Attributes && SlashOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

// 플레이어의 입력을 설정하는 함수
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동과 회전에 관한 입력을 바인딩합니다.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);

	// 점프, 장비 장착, 공격, 회피에 관한 입력을 바인딩합니다.
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASlashCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ASlashCharacter::Dodge);
}

// 점프 함수. 캐릭터가 아무런 액션에도 바인딩되지 않은 상태일 때만 점프합니다.
void ASlashCharacter::Jump()
{
	if (IsUnoccupied()) {
		Super::Jump();
	}
}


// 캐릭터가 현재 액션에 바인딩되어 있지 않은지 확인하는 함수
bool ASlashCharacter::IsUnoccupied()
{
	// 액션 상태가 Unoccupied인지 확인합니다.
	return ActionState == EActionState::EAS_Unoccupied;
}

// 데미지를 처리하는 함수
float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 데미지를 처리하고, HUD에 표시된 체력을 업데이트합니다.
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

// HUD에 표시된 체력을 업데이트하는 함수
void ASlashCharacter::SetHUDHealth()
{
	// 오버레이와 속성이 설정되어 있다면, 체력 바를 업데이트합니다.
	if (SlashOverlay && Attributes) {
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

// 히트 처리를 하는 함수
void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	// 무기의 충돌을 비활성화하고, 체력이 0이 아니라면 히트 반응 상태로 바꿉니다.
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}

// 오버랩되는 아이템을 설정하는 함수
void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	// OverlappingItem 변수에 아이템을 설정합니다.
	OverlappingItem = Item;
}

// 영혼을 추가하는 함수
void ASlashCharacter::AddSouls(ASoul* Soul)
{
	// 속성과 오버레이가 설정되어 있다면, 영혼을 추가하고 오버레이에 표시합니다.
	if (Attributes && SlashOverlay) {
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSouls(Attributes->GetSouls());
	}
}

// 골드를 추가하는 함수
void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	// 속성과 오버레이가 설정되어 있다면, 골드를 추가하고 오버레이에 표시합니다.
	if (Attributes && SlashOverlay) {
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

// 게임이 시작될 때 호출되는 함수
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 태그를 추가하고, 오버레이를 초기화합니다.
	Tags.Add(FName("EngageableTarget"));
	InitializeSlashOverlay();

}


// SlashOverlay를 초기화하는 함수
void ASlashCharacter::InitializeSlashOverlay()
{
	// 플레이어 컨트롤러를 가져옵니다.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		// SlashHUD를 가져옵니다.
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD) {
			// SlashOverlay를 설정하고, 속성이 있다면 오버레이의 체력, 스테미나, 골드, 영혼을 초기화합니다.
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes) {
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}

// 캐릭터를 전방으로 이동하는 함수
void ASlashCharacter::MoveForward(float Value)
{
	// 액션 상태가 Unoccupied가 아니라면 리턴합니다.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// 전방이 어디인지 확인합니다.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 입력 방향으로 이동합니다.
		AddMovementInput(Direction, Value);
	}
}

// 캐릭터를 오른쪽으로 이동하는 함수
void ASlashCharacter::MoveRight(float Value)
{
	// 액션 상태가 Unoccupied가 아니라면 리턴합니다.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// 오른쪽이 어디인지 확인합니다.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 입력 방향으로 이동합니다.
		AddMovementInput(Direction, Value);
	}
}

// 캐릭터를 회전하는 함수
void ASlashCharacter::Turn(float Value)
{
	// 입력된 값을 이용해 캐릭터를 회전시킵니다.
	AddControllerYawInput(Value);
}

// 캐릭터를 위아래로 본다
void ASlashCharacter::LookUp(float Value)
{
	// 입력된 값을 이용해 캐릭터를 위아래로 회전시킵니다.
	AddControllerPitchInput(Value);
}


// E 키를 눌렀을 때의 동작을 정의하는 함수
void ASlashCharacter::EKeyPressed()
{
	// OverlappingItem이 무기인지 검사합니다.
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		// 이미 무기를 장착하고 있다면, 기존 무기를 파괴합니다.
		if (EquippedWeapon) {
			EquippedWeapon->Destroy();
		}
		// 새로운 무기를 장착합니다.
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		// 무장 해제가 가능한지 검사하고, 가능하다면 무장 해제합니다.
		if (CanDisarm())
		{
			Disarm();
		}
		// 무장이 가능한지 검사하고, 가능하다면 무장합니다.
		else if (CanArm())
		{
			Arm();
		}
	}
}

// 공격을 수행하는 함수
void ASlashCharacter::Attack()
{
	Super::Attack();
	// 공격이 가능한지 검사하고, 가능하다면 공격 모션을 재생하고 액션 상태를 Attacking으로 설정합니다.
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

// 회피를 수행하는 함수
void ASlashCharacter::Dodge()
{
	// 현재 액션이 진행 중이거나 스테미나가 부족하면 함수를 종료합니다.
	if (IsOccupied() || !HasEnoughStamina()) return;
	// 회피 모션을 재생하고, 액션 상태를 Dodge로 설정합니다.
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && SlashOverlay)
	{
		// 회피에 필요한 스테미나를 소비하고, 스테미나 바를 갱신합니다.
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

// 스테미나가 충분한지 검사하는 함수
bool ASlashCharacter::HasEnoughStamina()
{
	// 현재 스테미나가 회피에 필요한 스테미나보다 많은지를 반환합니다.
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

// 현재 액션이 진행 중인지 검사하는 함수
bool ASlashCharacter::IsOccupied()
{
	// 액션 상태가 Unoccupied가 아니라면 true를 반환합니다.
	return ActionState != EActionState::EAS_Unoccupied;
}

// 무기를 장착하는 함수
void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	// 무기를 장착하고, 캐릭터 상태를 ECS_EquippedOneHandedWeapon으로 설정합니다.
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	// OverlappingItem과 EquippedWeapon을 갱신합니다.
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

// 공격을 종료하는 함수
void ASlashCharacter::AttackEnd()
{
	// 액션 상태를 Unoccupied로 설정합니다.
	ActionState = EActionState::EAS_Unoccupied;
}

// 회피를 종료하는 함수
void ASlashCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	// 액션 상태를 Unoccupied로 설정합니다.
	ActionState = EActionState::EAS_Unoccupied;
}


// 공격 가능 여부를 반환하는 함수
bool ASlashCharacter::CanAttack()
{
	// 액션 상태가 Unoccupied이고 캐릭터 상태가 Unequipped가 아닌 경우에만 공격 가능합니다.
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

// 무장 해제 가능 여부를 반환하는 함수
bool ASlashCharacter::CanDisarm()
{
	// 액션 상태가 Unoccupied이고 캐릭터 상태가 Unequipped가 아닌 경우에만 무장 해제 가능합니다.
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

// 무장 가능 여부를 반환하는 함수
bool ASlashCharacter::CanArm()
{
	// 액션 상태가 Unoccupied이고 캐릭터 상태가 Unequipped이며 장착된 무기가 있는 경우에만 무장 가능합니다.
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

// 무장 해제를 수행하는 함수
void ASlashCharacter::Disarm()
{
	// 무장 해제 애니메이션을 재생하고, 캐릭터 상태와 액션 상태를 갱신합니다.
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

// 무장을 수행하는 함수
void ASlashCharacter::Arm()
{
	// 무장 애니메이션을 재생하고, 캐릭터 상태와 액션 상태를 갱신합니다.
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

// 무기를 등 뒤에 부착하는 함수
void ASlashCharacter::AttachWeaponToBack()
{
	// 장착된 무기가 있다면 무기를 SpineSocket에 부착합니다.
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

// 무기를 손에 부착하는 함수
void ASlashCharacter::AttachWeaponToHand()
{
	// 장착된 무기가 있다면 무기를 RightHandSocket에 부착합니다.
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

// 무장 또는 무장 해제 애니메이션을 재생하는 함수
void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	// AnimInstance와 EquipMontage가 있다면 애니메이션을 재생하고, 해당 섹션으로 이동합니다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

// 캐릭터가 사망하는 함수
void ASlashCharacter::Die_Implementation()
{
	// 부모 클래스의 Die_Implementation을 호출하고, 액션 상태를 Dead로 설정합니다.
	// 또한 메쉬 충돌을 비활성화합니다.
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

// 무장이 끝나면 호출되는 함수
void ASlashCharacter::FinishEquipping()
{
	// 액션 상태를 Unoccupied로 설정합니다.
	ActionState = EActionState::EAS_Unoccupied;
}

// 히트 반응이 끝나면 호출되는 함수
void ASlashCharacter::HitReactEnd()
{
	// 액션 상태를 Unoccupied로 설정합니다.
	ActionState = EActionState::EAS_Unoccupied;
}

