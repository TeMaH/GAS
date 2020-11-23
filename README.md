Simple UE4 project with GameplayAbility System.
================================================

General overview
----------------

The main goal of this project is to demonstrate the issues we faced with Ability System. This project conceptually illustrates our approach to using GAS.
The ManualControl ability activates from server, bur runs on client. It listen local gamepad and applies Gameplay abilities on server. This ability activates by tag *Ability.ManualControll*. This ability can actiwate two other gameplay abilityes:
- GA1
- GA2

GA1 ability:
- activates by tag *Ability.GA1*
- cancel all abilities with tag *Ability* 
- applies GamePlay Effect GE1
    - this effect removes all effects with tag *Effect* and applies its own tag *Effect.GE1*
GA2 ability:
- activates by tag *Ability.GA2*
- cancel all abilities with tag *Ability* 
- applies GamePlay Effect GE2
    - this effect removes all effects with tag *Effect* and applies its own tag *Effect.GE2*

The *ManualControll* ability is activated in `void ACharacterController::OnPossess(APawn* InPawn)` method.
During its activation this ability creates ability task *UAbilityTask_InputHandler*. This task handles input from local gamepad and activates corresponded abilities (*GA1*, *GA2*) on server.

How to run
----------

Play mode:
- Number of Players: 1
- Net Mode: Play As Client

Press X/B on gemapad to activate one of two ability which applies one of two Gameplay Effect.

Press Y on gamepad  to switch characters.

Issues with GameplayAbility System we faced
-------------------------------------------

- We have added delay in 1 secont before activating the *ManualControll*. We did not managed to activate this ability on client side without this delay :(
- On the client the effect which we apply not always removes allready applied tag.

Moere details about tags removing. In case it runs as listen server - everything works correctly. We have only one tag per actor as we expect. See the picture below.
![Listen server](./misc/screanshots/OnePlayer_ListenServer.png)
In case it runs as Client the behaviour is different.
If you switch to another character and apply any Ability (GA1 or GA2) your character will have *two*(!) tags. In case you switch back to the first character and apply ability too you also will get two tags here. You could see such situation on the two screanshots bellow.

![Listen server](./misc/screanshots/OnePlayer_Client_02.png)

![Listen server](./misc/screanshots/OnePlayer_Client_03.png)

