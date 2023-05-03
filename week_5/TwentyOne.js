const fs = require('fs')
function extract_words(obj,file_path) {
  if ( typeof file_path != 'string' || ! file_path){
    console.log('File_path is needed.')
    obj['data'] = [];
    return;
  }

  try{
    inputD = fs.readFileSync(file_path);
  }catch(err){
    console.log('I/O Error during reading',file_path)
    obj['data'] = [];
    return;
  }
  data = inputD.toString().replace(/[^a-z0-9]/gi, ' ');
  data = data.toLowerCase();
  obj['data'] = data.split(' ');
}

function load_stop_words(obj){
  try{
  stopString = fs.readFileSync('../stop_words.txt')
  }catch(err){
    console.log('I/O error during reading stopword file');
    console.log(err);
    obj['stopwords'] = [];
    return
  }
  obj['stopwords'] = stopString.toString().split(',');
}

function increment_count(obj,word){
  if (typeof word != 'string'){
    console.log(word,'Type is not string')
    return};
  if (word in obj['fq']){
  obj['fq'][word] +=1 ;}
  else{
    obj['fq'][word] = 1;
  }
}

let data_storage_obj = {
  data: [],
  init: function (file_path){
    extract_words(this,file_path);
  },
  words:function (){return this.data;}
};

let stop_words_obj = {
  stopwords: [],
  init: function (){
    load_stop_words(stop_words_obj);
  },
  'is_stop_word':function (word){ 
    if (typeof word != 'string'){
      console.log(word,'is not String')
      return false}
    return stop_words_obj.stopwords.includes(word);}
};

let word_freqs_obj = {
  fq: {},
  increment_count(word){
     increment_count(word_freqs_obj,word) 
  },
  sorted: function(){
    let rawCounts = []
    for (let [word, frequency] of Object.entries(this.fq)) {
        rawCounts.push({word, frequency})
    }
    return rawCounts.sort(function(first, second) {
        return second.frequency - first.frequency;
    });
    
  }
  
}

data_storage_obj['init'](process.argv[2]);
stop_words_obj['init']();

data_storage_obj['words']().forEach(item => {
  if ((! stop_words_obj['is_stop_word'](item)) && item.length > 1){
    word_freqs_obj['increment_count'](item)
  }
});

fq_result = word_freqs_obj['sorted']();
for (let i = 0; i < Math.min(25,fq_result.length); i++) {
    console.log(fq_result[i].word, ' - ', fq_result[i].frequency)
}





