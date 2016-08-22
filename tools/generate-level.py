#!/usr/bin/python
import json

level = {
	'name': 'Generated level',
	'starting-lives': 10,
	'starting-money': 100,
	'grid-size': [16, 16],
	'goal':	[8, 8],
	'waves': []
}

time = 3
for i in range(10):
	creepdata = { 
		'type': 'GenericCreep',
		'hp': 40*(i+1),
		'bounty': 10*(i+1),
		'spawn-at': [0, 0],
		'spawn-time': {
			'start': 0.0,
			'count': 5*(i+1),
			'interval': 2.0,
		}
	}
	def creeps_at(data, pos):
		creepdata2 = data.copy()
		creepdata2['spawn-at'] = pos
		return creepdata2
	level['waves'].append({
		'start-time': time,
		'creeps': [
			creeps_at(creepdata, [0, 0]),
			creeps_at(creepdata, [15, 15])
		]
	})
	time += 5*(i+1)*2+10

print(json.dumps(level, indent=4))
