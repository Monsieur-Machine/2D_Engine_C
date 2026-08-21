# What to do for the 2D Engine

### Todo

###### Features

- [ ] Change linked list for each implementation to a macro (iykyk)

- [ ] Add compression into asset builder and asset manager

- [ ] Make functionnal hitboxes between entities and map

- [ ] Make triggers
  - [ ] Add triggers into map loading

- [ ] Make physics component use acceleration

- [ ] Make camera use acceleration

###### Bugs

- [ ] Fix transparency issues
  - [ ] Rethink export properties and platforms ?

### In progress

### Done

- [x] Make camera center on map

- [x] Create camera

- [x] Optimize asset builder file analize

- [x] Create tilemap management and display
  - [x] Find tilemap building format
  - [x] Add building support for tilemaps in asset builder
  - [x] Add loading support in asset manager
  - [x] Display tilemaps
    - [ ] Work on optimizations !

- [x] Work on entities draw order

- [x] Make entities react to events

- [x] Work on events (publish / subscribe)

- [x] Work on asset corruption

- [x] Make functionnal hitboxes between entities

- [x] Change the way default values are defined across components and systems
  - [x] Remove the need of a dummy texture

- [x] Work on linked lists optimizations
