class Set {
	constructor() {
		this.inputs = [];
		this.hardcodedPoly = false;
		this.poly = false;
		this.checked = false;
	}
}

var sets = {};

sets.b = new Set();
sets.c = new Set();
sets.d = new Set();
sets.e = new Set();
sets.f = new Set();
sets.g = new Set();
sets.h = new Set();
sets.i = new Set();
sets.j = new Set();
sets.k = new Set();
sets.g.hardcodedPoly = true;

sets.b.inputs.push(sets.c);
sets.c.inputs.push(sets.d);
sets.d.inputs.push(sets.e);
sets.e.inputs.push(sets.f);
sets.f.inputs.push(sets.g);
sets.d.inputs.push(sets.h);
sets.h.inputs.push(sets.i);
sets.i.inputs.push(sets.j);
sets.j.inputs.push(sets.k);

function checkSet(set) {
	var returnVal = set.hardcodedPoly;
	for(var x=0;x<set.inputs.length;x++) {
		checkSet(set.inputs[x]);
		if(set.inputs[x].poly) returnVal = true;
	}
	set.checked = true;
	set.poly = returnVal;
}

checkSet(sets.b);
console.log(sets);
