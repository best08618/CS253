const fs = require('fs');
var prompt = require('prompt-sync')();

var words_col = [[],null];
var stopwords_col = [[],null];
var none_stopwords_col = [[],
  () =>
    words_col[0].map((w) => {
      if (!stopwords_col[0].includes(w) && w.length >1) {
        return w;
      } else {
        return '';
      }
    })
];
var unique_words = [[],
                   ()=> Array.from(new Set(none_stopwords_col[0].filter(x => x)))]

// var counts = [[],
//   () => 
//     unique_words[0].map((w) =>
//       none_stopwords_col[0].filter((word) => word === w).length
//     ),
// ];

var counts = [[],
     () => Object.entries(none_stopwords_col[0].reduce(function (acc, curr) {
       if (unique_words[0].includes(curr))
  return acc[curr] ? ++acc[curr] : acc[curr] = 1, acc
       else 
         return acc
}, {}))];
  

                  
var all_cols= [words_col,stopwords_col,none_stopwords_col,unique_words,counts]


function update(){
  all_cols.forEach(c=>{
    if (c[1] !=null){
      c[0] = c[1]();
    }
  });
}



function print(){
    all_cols.forEach(c=>{
      console.log(c[0]);
  });
  sorted_info = counts[0].sort(function(a,b){return b[1] - a[1];})
    for (let i = 0; i <  Math.min(25,sorted_info.length); i++){
      console.log(sorted_info[i][0], ' - ', sorted_info[i][1]);
}
}




stopwords_col[0] = fs.readFileSync('../stop_words.txt').toString().split(',');
while(true){
var file_path = prompt("FIle path: ")
words_col[0] = fs.readFileSync(file_path).toString().replace(/[^a-z0-9]/gi, ' ').toLowerCase().split(' ').filter(function(n){return n; });
update()
print()
}
