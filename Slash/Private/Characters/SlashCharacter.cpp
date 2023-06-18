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

// ������ �Լ�
ASlashCharacter::ASlashCharacter()
{
	// �� Actor�� �� ƽ���� ������Ʈ�Ǿ�� �մϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ��� ȸ���� ������� �ʽ��ϴ�.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ������ �̵� �������� ȸ���ϵ��� �����մϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// ĳ������ �޽��� �浹 ������ �����մϴ�.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECCollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECCollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// ī�޶�� ī�޶� ���� �����մϴ�.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

}

// �� ƽ���� ȣ��Ǵ� �Լ�
void ASlashCharacter::Tick(float DeltaTime)
{
	// �Ӽ��� �������̰� �����Ǿ� �ִٸ�, ���¹̳��� ȸ���ϰ� �������̿� ǥ���մϴ�.
	if (Attributes && SlashOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

// �÷��̾��� �Է��� �����ϴ� �Լ�
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �̵��� ȸ���� ���� �Է��� ���ε��մϴ�.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);

	// ����, ��� ����, ����, ȸ�ǿ� ���� �Է��� ���ε��մϴ�.
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASlashCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ASlashCharacter::Dodge);
}

// ���� �Լ�. ĳ���Ͱ� �ƹ��� �׼ǿ��� ���ε����� ���� ������ ���� �����մϴ�.
void ASlashCharacter::Jump()
{
	if (IsUnoccupied()) {
		Super::Jump();
	}
}


// ĳ���Ͱ� ���� �׼ǿ� ���ε��Ǿ� ���� ������ Ȯ���ϴ� �Լ�
bool ASlashCharacter::IsUnoccupied()
{
	// �׼� ���°� Unoccupied���� Ȯ���մϴ�.
	return ActionState == EActionState::EAS_Unoccupied;
}

// �������� ó���ϴ� �Լ�
float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// �������� ó���ϰ�, HUD�� ǥ�õ� ü���� ������Ʈ�մϴ�.
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

// HUD�� ǥ�õ� ü���� ������Ʈ�ϴ� �Լ�
void ASlashCharacter::SetHUDHealth()
{
	// �������̿� �Ӽ��� �����Ǿ� �ִٸ�, ü�� �ٸ� ������Ʈ�մϴ�.
	if (SlashOverlay && Attributes) {
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

// ��Ʈ ó���� �ϴ� �Լ�
void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	// ������ �浹�� ��Ȱ��ȭ�ϰ�, ü���� 0�� �ƴ϶�� ��Ʈ ���� ���·� �ٲߴϴ�.
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}

// �������Ǵ� �������� �����ϴ� �Լ�
void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	// OverlappingItem ������ �������� �����մϴ�.
	OverlappingItem = Item;
}

// ��ȥ�� �߰��ϴ� �Լ�
void ASlashCharacter::AddSouls(ASoul* Soul)
{
	// �Ӽ��� �������̰� �����Ǿ� �ִٸ�, ��ȥ�� �߰��ϰ� �������̿� ǥ���մϴ�.
	if (Attributes && SlashOverlay) {
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSouls(Attributes->GetSouls());
	}
}

// ��带 �߰��ϴ� �Լ�
void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	// �Ӽ��� �������̰� �����Ǿ� �ִٸ�, ��带 �߰��ϰ� �������̿� ǥ���մϴ�.
	if (Attributes && SlashOverlay) {
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

// ������ ���۵� �� ȣ��Ǵ� �Լ�
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �±׸� �߰��ϰ�, �������̸� �ʱ�ȭ�մϴ�.
	Tags.Add(FName("EngageableTarget"));
	InitializeSlashOverlay();

}


// SlashOverlay�� �ʱ�ȭ�ϴ� �Լ�
void ASlashCharacter::InitializeSlashOverlay()
{
	// �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		// SlashHUD�� �����ɴϴ�.
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD) {
			// SlashOverlay�� �����ϰ�, �Ӽ��� �ִٸ� ���������� ü��, ���׹̳�, ���, ��ȥ�� �ʱ�ȭ�մϴ�.
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

// ĳ���͸� �������� �̵��ϴ� �Լ�
void ASlashCharacter::MoveForward(float Value)
{
	// �׼� ���°� Unoccupied�� �ƴ϶�� �����մϴ�.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// ������ ������� Ȯ���մϴ�.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// �Է� �������� �̵��մϴ�.
		AddMovementInput(Direction, Value);
	}
}

// ĳ���͸� ���������� �̵��ϴ� �Լ�
void ASlashCharacter::MoveRight(float Value)
{
	// �׼� ���°� Unoccupied�� �ƴ϶�� �����մϴ�.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// �������� ������� Ȯ���մϴ�.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// �Է� �������� �̵��մϴ�.
		AddMovementInput(Direction, Value);
	}
}

// ĳ���͸� ȸ���ϴ� �Լ�
void ASlashCharacter::Turn(float Value)
{
	// �Էµ� ���� �̿��� ĳ���͸� ȸ����ŵ�ϴ�.
	AddControllerYawInput(Value);
}

// ĳ���͸� ���Ʒ��� ����
void ASlashCharacter::LookUp(float Value)
{
	// �Էµ� ���� �̿��� ĳ���͸� ���Ʒ��� ȸ����ŵ�ϴ�.
	AddControllerPitchInput(Value);
}


// E Ű�� ������ ���� ������ �����ϴ� �Լ�
void ASlashCharacter::EKeyPressed()
{
	// OverlappingItem�� �������� �˻��մϴ�.
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		// �̹� ���⸦ �����ϰ� �ִٸ�, ���� ���⸦ �ı��մϴ�.
		if (EquippedWeapon) {
			EquippedWeapon->Destroy();
		}
		// ���ο� ���⸦ �����մϴ�.
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		// ���� ������ �������� �˻��ϰ�, �����ϴٸ� ���� �����մϴ�.
		if (CanDisarm())
		{
			Disarm();
		}
		// ������ �������� �˻��ϰ�, �����ϴٸ� �����մϴ�.
		else if (CanArm())
		{
			Arm();
		}
	}
}

// ������ �����ϴ� �Լ�
void ASlashCharacter::Attack()
{
	Super::Attack();
	// ������ �������� �˻��ϰ�, �����ϴٸ� ���� ����� ����ϰ� �׼� ���¸� Attacking���� �����մϴ�.
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

// ȸ�Ǹ� �����ϴ� �Լ�
void ASlashCharacter::Dodge()
{
	// ���� �׼��� ���� ���̰ų� ���׹̳��� �����ϸ� �Լ��� �����մϴ�.
	if (IsOccupied() || !HasEnoughStamina()) return;
	// ȸ�� ����� ����ϰ�, �׼� ���¸� Dodge�� �����մϴ�.
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && SlashOverlay)
	{
		// ȸ�ǿ� �ʿ��� ���׹̳��� �Һ��ϰ�, ���׹̳� �ٸ� �����մϴ�.
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

// ���׹̳��� ������� �˻��ϴ� �Լ�
bool ASlashCharacter::HasEnoughStamina()
{
	// ���� ���׹̳��� ȸ�ǿ� �ʿ��� ���׹̳����� �������� ��ȯ�մϴ�.
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

// ���� �׼��� ���� ������ �˻��ϴ� �Լ�
bool ASlashCharacter::IsOccupied()
{
	// �׼� ���°� Unoccupied�� �ƴ϶�� true�� ��ȯ�մϴ�.
	return ActionState != EActionState::EAS_Unoccupied;
}

// ���⸦ �����ϴ� �Լ�
void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	// ���⸦ �����ϰ�, ĳ���� ���¸� ECS_EquippedOneHandedWeapon���� �����մϴ�.
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	// OverlappingItem�� EquippedWeapon�� �����մϴ�.
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

// ������ �����ϴ� �Լ�
void ASlashCharacter::AttackEnd()
{
	// �׼� ���¸� Unoccupied�� �����մϴ�.
	ActionState = EActionState::EAS_Unoccupied;
}

// ȸ�Ǹ� �����ϴ� �Լ�
void ASlashCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	// �׼� ���¸� Unoccupied�� �����մϴ�.
	ActionState = EActionState::EAS_Unoccupied;
}


// ���� ���� ���θ� ��ȯ�ϴ� �Լ�
bool ASlashCharacter::CanAttack()
{
	// �׼� ���°� Unoccupied�̰� ĳ���� ���°� Unequipped�� �ƴ� ��쿡�� ���� �����մϴ�.
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

// ���� ���� ���� ���θ� ��ȯ�ϴ� �Լ�
bool ASlashCharacter::CanDisarm()
{
	// �׼� ���°� Unoccupied�̰� ĳ���� ���°� Unequipped�� �ƴ� ��쿡�� ���� ���� �����մϴ�.
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

// ���� ���� ���θ� ��ȯ�ϴ� �Լ�
bool ASlashCharacter::CanArm()
{
	// �׼� ���°� Unoccupied�̰� ĳ���� ���°� Unequipped�̸� ������ ���Ⱑ �ִ� ��쿡�� ���� �����մϴ�.
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

// ���� ������ �����ϴ� �Լ�
void ASlashCharacter::Disarm()
{
	// ���� ���� �ִϸ��̼��� ����ϰ�, ĳ���� ���¿� �׼� ���¸� �����մϴ�.
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

// ������ �����ϴ� �Լ�
void ASlashCharacter::Arm()
{
	// ���� �ִϸ��̼��� ����ϰ�, ĳ���� ���¿� �׼� ���¸� �����մϴ�.
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

// ���⸦ �� �ڿ� �����ϴ� �Լ�
void ASlashCharacter::AttachWeaponToBack()
{
	// ������ ���Ⱑ �ִٸ� ���⸦ SpineSocket�� �����մϴ�.
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

// ���⸦ �տ� �����ϴ� �Լ�
void ASlashCharacter::AttachWeaponToHand()
{
	// ������ ���Ⱑ �ִٸ� ���⸦ RightHandSocket�� �����մϴ�.
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

// ���� �Ǵ� ���� ���� �ִϸ��̼��� ����ϴ� �Լ�
void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	// AnimInstance�� EquipMontage�� �ִٸ� �ִϸ��̼��� ����ϰ�, �ش� �������� �̵��մϴ�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

// ĳ���Ͱ� ����ϴ� �Լ�
void ASlashCharacter::Die_Implementation()
{
	// �θ� Ŭ������ Die_Implementation�� ȣ���ϰ�, �׼� ���¸� Dead�� �����մϴ�.
	// ���� �޽� �浹�� ��Ȱ��ȭ�մϴ�.
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

// ������ ������ ȣ��Ǵ� �Լ�
void ASlashCharacter::FinishEquipping()
{
	// �׼� ���¸� Unoccupied�� �����մϴ�.
	ActionState = EActionState::EAS_Unoccupied;
}

// ��Ʈ ������ ������ ȣ��Ǵ� �Լ�
void ASlashCharacter::HitReactEnd()
{
	// �׼� ���¸� Unoccupied�� �����մϴ�.
	ActionState = EActionState::EAS_Unoccupied;
}

