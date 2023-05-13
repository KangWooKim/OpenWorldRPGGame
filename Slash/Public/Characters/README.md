# BaseCharacter.h


ABaseCharacter::ABaseCharacter(): 클래스의 생성자입니다. 여기서는 클래스의 초기 설정을 합니다. 각 속성을 초기화하고, 충돌 응답을 설정합니다.

ABaseCharacter::BeginPlay(): 게임이 시작될 때 호출되는 함수입니다.

ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter): 캐릭터가 공격을 받았을 때 호출되는 함수입니다. 공격받은 위치와 공격자를 파라미터로 받아, 해당 위치에서 피격 사운드와 파티클을 생성하고, 피격 애니메이션을 실행합니다.

ABaseCharacter::Attack(): 캐릭터가 공격하는 동작을 정의하는 함수입니다.

ABaseCharacter::Die_Implementation(): 캐릭터가 죽었을 때 호출되는 함수입니다. 죽음 애니메이션을 실행하고, 캐릭터에 "Dead" 태그를 추가합니다.

ABaseCharacter::PlayHitReactMontage(const FName& SectionName): 피격 반응 애니메이션을 재생하는 함수입니다. 애니메이션 섹션의 이름을 파라미터로 받습니다.

ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint): 공격받은 방향에 따라 피격 반응 애니메이션을 결정하고 재생하는 함수입니다.

ABaseCharacter::PlayHitSound(const FVector& ImpactPoint): 피격 사운드를 재생하는 함수입니다.

ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint): 피격 위치에 파티클을 생성하는 함수입니다.

ABaseCharacter::HandleDamage(float DamageAmount): 데미지를 처리하는 함수입니다.

ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName): 특정 애니메이션 몽타주의 특정 섹션을 재생하는 함수입니다.

ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames): 애니메이션 몽타주의 여러 섹션 중 하나를 무작위로 선택하여 재생하는 함수입니다.

ABaseCharacter::PlayAttackMontage(): 공격 애니메이션을 재생하는 함수입니다.

ABaseCharacter::PlayDeathMontage(): 죽음 애니메이션을 재생하는 함수입니다.

ABaseCharacter::PlayDodgeMontage(): 회피 애니메이션을 재생하는 함수입니다.
  
ABaseCharacter::StopAttackMontage(): 공격 애니메이션을 중지하는 함수입니다.

ABaseCharacter::GetTranslationWarpTarget(): 캐릭터가 이동하려는 위치를 계산하는 함수입니다. 이는 보통 전투 상황에서 적 캐릭터에게 접근하기 위해 사용됩니다.

ABaseCharacter::GetRotationWarpTarget(): 캐릭터가 회전하려는 목표 위치를 반환하는 함수입니다. 이는 보통 전투 상황에서 적 캐릭터를 바라보기 위해 사용됩니다.

ABaseCharacter::DisableCapsule(): 캐릭터의 캡슐 충돌체를 비활성화하는 함수입니다. 이는 주로 캐릭터가 죽었을 때 호출됩니다.

ABaseCharacter::CanAttack(): 캐릭터가 공격 가능한 상태인지를 판단하는 함수입니다.

ABaseCharacter::IsAlive(): 캐릭터가 살아있는지를 판단하는 함수입니다.

ABaseCharacter::DisableMeshCollision(): 캐릭터의 메시 충돌을 비활성화하는 함수입니다.

ABaseCharacter::AttackEnd(): 캐릭터의 공격이 끝났을 때 호출되는 함수입니다.

ABaseCharacter::DodgeEnd(): 캐릭터의 회피 동작이 끝났을 때 호출되는 함수입니다.

ABaseCharacter::Tick(float DeltaTime): 게임이 진행됨에 따라 매 프레임마다 호출되는 함수입니다.

ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled): 캐릭터의 무기 충돌을 설정하는 함수입니다. 이 함수는 무기가 다른 객체와 충돌할 수 있는지 여부를 설정합니다.
  
  ---

  
# SlashCharacter.h
  
ASlashCharacter::ASlashCharacter(): 클래스의 생성자입니다. 이곳에서는 기본 설정과 객체의 초기화를 수행합니다.

ASlashCharacter::Tick(float DeltaTime): 매 프레임마다 호출되며, 스태미나의 회복과 HUD 업데이트를 수행합니다.

ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent): 플레이어의 입력을 처리하는 함수입니다. 여기서는 입력에 따른 함수를 바인딩합니다.

ASlashCharacter::Jump(): 플레이어의 점프 동작을 처리하는 함수입니다.

ASlashCharacter::IsUnoccupied(): 플레이어가 어떤 행동도 하지 않고 있는지를 확인하는 함수입니다.

ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser): 플레이어가 데미지를 받았을 때의 동작을 처리하는 함수입니다.

ASlashCharacter::SetHUDHealth(): HUD의 체력 바를 설정하는 함수입니다.

ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter): 플레이어가 공격을 받았을 때의 동작을 처리하는 함수입니다.

ASlashCharacter::SetOverlappingItem(AItem* Item): 플레이어가 현재 겹치는 아이템을 설정하는 함수입니다.

ASlashCharacter::AddSouls(ASoul* Soul): 플레이어가 소울을 획득했을 때의 동작을 처리하는 함수입니다.

ASlashCharacter::AddGold(ATreasure* Treasure): 플레이어가 금을 획득했을 때의 동작을 처리하는 함수입니다.

ASlashCharacter::BeginPlay(): 게임이 시작될 때 호출되는 함수입니다. 여기서는 초기 설정과 HUD의 초기화를 수행합니다.

ASlashCharacter::InitializeSlashOverlay(): SlashOverlay를 초기화하는 함수입니다.

ASlashCharacter::MoveForward(float Value), ASlashCharacter::MoveRight(float Value), ASlashCharacter::Turn(float Value), ASlashCharacter::LookUp(float Value): 각각 플레이어의 전방 이동, 좌우 이동, 회전, 상하 뷰 변경을 담당하는 함수입니다.

ASlashCharacter::EKeyPressed(): 'E' 키를 눌렀을 때의 동작을 처리하는 함수입니다. 무기 장착 및 해제를 담당합니다.

ASlashCharacter::Attack(): 공격 동작을 처리하는 함수입니다.
  
ASlashCharacter::Dodge(): 회피 동작을 처리하는 함수입니다. 회피 동작 중에는 스태미나가 소모됩니다.

ASlashCharacter::HasEnoughStamina(): 스태미나가 충분한지 확인하는 함수입니다.

ASlashCharacter::IsOccupied(): 캐릭터가 특정 동작(공격, 회피 등)을 수행 중인지 확인하는 함수입니다.

ASlashCharacter::EquipWeapon(AWeapon* Weapon): 무기를 장착하는 함수입니다.

ASlashCharacter::AttackEnd(): 공격 동작이 끝났음을 알리는 함수입니다.

ASlashCharacter::DodgeEnd(): 회피 동작이 끝났음을 알리는 함수입니다.

ASlashCharacter::CanAttack(): 공격이 가능한 상태인지를 확인하는 함수입니다.

ASlashCharacter::CanDisarm(): 무기를 해제할 수 있는 상태인지 확인하는 함수입니다.

ASlashCharacter::CanArm(): 무기를 장착할 수 있는 상태인지 확인하는 함수입니다.

ASlashCharacter::Disarm(): 무기를 해제하는 함수입니다.

ASlashCharacter::Arm(): 무기를 장착하는 함수입니다.

ASlashCharacter::AttachWeaponToBack(): 무기를 캐릭터의 등에 부착하는 함수입니다.

ASlashCharacter::AttachWeaponToHand(): 무기를 캐릭터의 손에 부착하는 함수입니다.

ASlashCharacter::PlayEquipMontage(const FName& SectionName): 무기 장착 애니메이션을 재생하는 함수입니다.

ASlashCharacter::Die_Implementation(): 캐릭터의 사망을 처리하는 함수입니다.

ASlashCharacter::FinishEquipping(): 무기 장착이 완료되었음을 알리는 함수입니다.

ASlashCharacter::HitReactEnd(): 피격 반응이 끝났음을 알리는 함수입니다.
  
---
  
# SlashAnimInstance.h
  
USlashAnimInstance::NativeInitializeAnimation(): 이 함수는 애니메이션 인스턴스가 초기화될 때 호출됩니다. 이 함수에서는 SlashCharacter를 ASlashCharacter 타입으로 형 변환하여 저장하고, SlashCharacterMovement를 이 캐릭터의 움직임 컴포넌트로 설정합니다. 이렇게 함으로써, 애니메이션 인스턴스는 캐릭터의 움직임 상태를 알 수 있게 됩니다.

USlashAnimInstance::NativeUpdateAnimation(float DeltaTime): 이 함수는 애니메이션 인스턴스가 업데이트될 때마다 호출됩니다. 이 함수에서는 캐릭터의 움직임 속도(GroundSpeed), 캐릭터가 떨어지고 있는지(IsFalling), 그리고 캐릭터의 상태(CharacterState와 ActionState)와 사망 포즈(DeathPose)를 업데이트합니다. 이렇게 함으로써, 애니메이션 인스턴스는 캐릭터의 현재 상태에 따라 애니메이션을 업데이트할 수 있게 됩니다.
