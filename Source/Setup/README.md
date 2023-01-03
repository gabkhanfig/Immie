# How to Setup Immie Project

## Requirements

> Windows 10+
> Visual Studio
> GitHub account
> Epic Games account
> Unreal Engine 5.1 (built from Epic Games source)

## Visual Studio Setup

[Download visual studio community edition](https://visualstudio.microsoft.com/free-developer-offers/). 
The workloads necessary are the following:
- .NET desktop development
- Desktop development with C++
- Game development with C++


The individual components necessary to add on are the following:
- .NET Core 3.1 Runtime (LTS)

## Unreal Engine Setup

[Connect GitHub account to Epic Games account](https://www.unrealengine.com/account/connections?lang=en-US). Follow the prompt in the recieved email to join the Epic Games organization. You should now have access to the [Unreal Engine Source Code Repo](https://github.com/EpicGames/UnrealEngine). Clone the Unreal Engine sourcee code into a new UnrealEngine directory.
```git clone https://github.com/EpicGames/UnrealEngine.git``` 
Cd into it.
```cd UnrealEngine``` 
Checkout the correct unreal engine version. These is `major`.`minor`.
```git checkout 5.1```
Execute the setup and generate project files batch files.
```
Setup.bat
GenerateProjectFiles.bat
```
Open the `UE5.sln` file. Ensure the Solution Configurations is `Development Editor` and then Solution Platforms is set to `Win64`. Right click on `UE5` under the engine folder and select `Build`. This usually takes a while. On a ryzen 9 7950x it took ~30 minutes. Double check everything worked by navigating to `UnrealEngine\Engine\Binaries\Win64` and running `UnrealEditor.exe`. Optionally it can be run through Visual Studio. Right click again on `UE5` and select `Set as Startup Project`. Running local windows debuggers or pressing f5 should successfully launch the engine.

## Immie Setup

Right click on `Immie.uproject`. Select `Switch Unreal Engine version...` and select the version (UnrealEngine folder) setup above. Next select `Generated Visual Studio project files`. Once these are complete, open `Immie.sln`. Right click on `Immie` under the `Games` folder within the solution. Click `Build`. Immie can now be launched either through Visual Studio or by running `Immie.uproject`.