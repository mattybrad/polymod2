class Set {
	constructor() {
		this.inputs = [];
		this.hardcodedPoly = false;
		this.poly = false;
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
sets.l = new Set();
sets.m = new Set();
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
sets.k.inputs.push(sets.c);
sets.j.inputs.push(sets.l);
sets.l.inputs.push(sets.m);
sets.l.inputs.push(sets.l);
sets.d.inputs.push(sets.d);

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

checkSet(sets.b);
console.log(sets);
