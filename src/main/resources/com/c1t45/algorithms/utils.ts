` Instructions!:
A = 0,2,4,6, ... Even Row - Top
	has 1 on top, 2 on bottoms
B = Odd Row
	has 2 on tops, 1 on bottom
8 is A [4,12,13]
12 is B [8,7,17]
`


// 56 isnt real
const verteciesInitials = [0,3,7,11,16,21,27,35,40,45,49,53, 56];

// 0,3,7,11,16,21,27,33,38,43,47,51
// 0,1,2,3,4,5,6,7,8,09,10,11,12,13
const hexagonsInitials = [0,3,7,12,16];

console.clear();

function getRowOffset(dot:number):number{
    let i = 0;
    for (i = 1; i < verteciesInitials.length-1 && verteciesInitials[i] <= dot; i++);
    return i-1;
}

function devide(a:number,b:number){
    return Math.floor(a/b);
}

function nearVertecies(dot:number, debug:boolean = true):Array<number>{

    if (debug) console.log("vertex == " + dot)
    const vertexRow = getRowOffset(dot);
    
    const offset = dot-verteciesInitials[vertexRow];
    const offsetMinusOne = Math.max(0, offset-1);
    const offsetPlusOne = Math.min(verteciesInitials[vertexRow+1] - verteciesInitials[vertexRow], offset+1);

    const above = Math.max(vertexRow - 1, 0);
    const below = Math.min(vertexRow + 1, verteciesInitials.length-2);
     
    function modify(addon:number, axis:number){
        return Math.min(verteciesInitials[axis+1]-1, Math.max(verteciesInitials[axis], verteciesInitials[axis]+ offset + addon))
    }

    const isEvan = vertexRow % 2 == 0;
    
    // const isEndRow = 

    const dots = new Set<number>();

    switch (vertexRow){
        default:
            const middle_fix_7 = vertexRow === 6 ? 1 : vertexRow === 7 ? -1 : 0;
            const middle_fix_5 = vertexRow === 5 ? 1 : vertexRow === 6 ?  -1 : 0;

            dots.add(modify(-middle_fix_7 ,above));
            dots.add(modify(middle_fix_5 ,below));
            dots.add(modify(((vertexRow < 6  ? !isEvan : isEvan) ?  -1: 1) - middle_fix_7,isEvan ? below : above));
            break;
    }
    dots.delete(dot)
    const arr = Array.from(dots.values()) as Array<number>;
    if (debug)
    console.log(`---- ${JSON.stringify(arr)} ; evn:${isEvan} ; rw:${vertexRow}(${verteciesInitials[vertexRow]}) o:${offset}`);
    return arr;
}

function testVertecies(){
    nearVertecies(12) // [7, 18, 8]   ; even:true  ; r:3 o:1 | V
    nearVertecies(17) // [12, 22, 23] ; even:true  ; r:4 o:1 | V

    nearVertecies(22) // [16, 17, 29]    ; even:false ; r:5 o:1 | X
    nearVertecies(29) // [22, 35, 36]    ; even:true  ; r:6 o:2 | X
    nearVertecies(36) // [29, 30, 41]    ; even:false ; r:7 o:1 | X

    nearVertecies(41) // [45, 46, 36] ; even:false ; r:8 o:1 | V
    nearVertecies(46) // [41, 42, 50] ; even:false ; r:9 o:1 | V
}


// function nearAreas(dot:number):Array<number>{
//     const array:Array<number> = [];
    
//     const vertexRow = getRowOffset(dot);
//     const hexRow = devide(vertexRow,2);

//     const above = Math.max(hexRow - 1, 0);
//     const below = Math.min(hexRow + 1, 5);

//     switch(vertexRow){
//         case 0:
//             return [dot];
//         case 11:
//             return [dot - 51+16];
//         default:
            
//     }

//     if (hexRow === 0){

//     }

//     return array;
// }