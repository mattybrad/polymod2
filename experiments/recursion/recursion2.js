class Set {
	constructor() {
		this.inputs = [];
		this.hardcodedPoly = false;
		this.poly = false;
	}
}

var sets = {};

sets.a = new Set();
sets.b = new Set();

sets.a.inputs.push(sets.b);
sets.b.inputs.push(sets.b);
sets.b.hardcodedPoly = true;

function checkSet(set) {
	if(set.hardcodedPoly) set.poly = true;
	var x;
	for(x=0;x<set.inputs.length;x++) {
		if(set.inputs[x].poly) set.poly = true;
	}
	if(!set.poly) {
		for(x=0;x<set.inputs.length;x++) {
			if(set!=set.inputs[x]) checkSet(set.inputs[x]);
			if(set.inputs[x].poly) set.poly = true;
		}
	}
}

checkSet(sets.a);
console.log(sets);
