# Emergent Storytelling Tool
UE5 tool that generates subsequent scenarios given actions, agents, locations and objects.

![image](https://github.com/alessianigretti/emergent_storytelling_tool/assets/17513294/07d98767-c20a-4b8f-89f9-c0ad08b82459)

## Table of contents
- [Introduction](#introduction)
- [Components](#components)
  - [Interface](#interface)
  - [Data](#data)
  - [Authoring Language](#authoring-language)
- [Usage Instructions](#usage-instructions)
- [Known Issues](#known-issues)
- [Future Work](#future-work)
   
## Introduction
This Unreal Engine project is a UI tool that allows to visualise the effects of a sequence of actions executed on a set of agents via the use of smart objects in predetermined locations. Actions are selected based on their requirements, which are represented by the presence of traits in an agent or objects in a location. Agents, items and actions are fully authorable, and the sequence of events that can be created is virtually infinite and entirely dependent on the authoring of the data.

I developed an initial version of this tool made with Unity during my time at Klang Games with David Oppenberg (Game Designer), and I recently decided to repurpose it for Unreal Engine as a personal project. Shout-out to Oddur Magnusson and the team at Klang Games for allowing me to freely share this, and to every friend who had fun playing with it and helping me author a working version of the initial data used.

## Components
The tool is comprised of 3 main elements:
* UI: the main interface that allows users to visualise the occurrences developed in the game world
* Datatables: the assets in which the data is imported, and can be visualised and (temporarily) modified
* CSV files: the files in which the final data is written (this overwrites any data in datatables)

### Interface
The interface is simple and easily visualisable. It comprises of:
1. Main text area, in which the story is written
2. Generate and Generate x10 buttons, to generate either 1 or 10 lines of story at a time (effects are applied at each printed line)
3. Reimport button, to allow users to update data without needing to restart PIE
4. Clear button, to clear the text area (the effects of the story won't be reset)
5. Agents tab, to visualise the state of each agent (it updates after each action)
6. Items tab, to visualise all locations and objects in the game world
7. Actions tab, to visualise all possible selectable actions

![image](https://github.com/alessianigretti/emergent_storytelling_tool/assets/17513294/74e5bec3-1533-4f75-8412-8d2aab34bbe1)

### Data
The data is authored via 3 files:
* actions.csv: it contains a column for the name of the action, and a column for the description which includes a location (if needed) and a list of requirements that participating agents need to have, followed by a list of effects that the action has on the participating agents
* agents.csv: it contains a column for the name of the agent, and a column for the traits that an agent has (including their current location and any possession)
* items.csv: it contains a column for the name of the item, whether it is a location or an object, and any requisite to either be in the location or interact with the object

The CSV file is imported into the corresponding datatable (`DT_Actions`, `DT_Agents` and `DT_Items`) in Unreal, and can be visualised and temporarily modified through the editor.

![image](https://github.com/alessianigretti/emergent_storytelling_tool/assets/17513294/8a84f3f5-2178-4f98-b69e-1d5b795b01c8)

### Authoring Language
Here is a reference for the language used to author the data.
* Name_: prefix to each action, agent or item name
* L_: prefix to locations
* O_: prefix to objects
* T_: prefix to traits
* P_: prefix to possessions
* N_: prefix for negation
* =: separator between requirements and effects
* A_: prefix to added requirement
* R_: prefix to removed requirement
* A1_, A2_: prefix to indicate an agent in the case of multiple agents participating in an action

## Usage Instructions
In order to author new data, simple steps are required (Unreal can stay open during each of these steps):
1. Open the relevant csv file (i.e. actions.csv)
2. Edit and save the csv file
3. Open the relevant datatable asset in Unreal (i.e. DT_Actions)
4. Click Reimport and then save the asset
5. Launch the simulation

If the simulation is already running and new data is being authored in the meantime, you can perform steps 1-4 and then click the Reimport button during PIE.

## Known Issues
_No known issues yet, though multi-agent interactions might not work well yet._

## Future Work
* Implement Export button in the UI to allow for easy export of generated stories
* Implement Reset button in the UI to reset agents and world to their original values
* Improve readability of authoring data
* Implement quantities
