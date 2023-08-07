-- fac:n ==> n!
{zero? eq@[id,%0] }
{sub1 -@[id,%1]}
{fac (zero?->%1;*@[id,fac@sub1])}
fac:10

-- iota:n ==> <1,2,3,...,n>
{one? eq@[id,%1]}
{iota (one?->[id];apndr@[iota@sub1,id])}
iota:6

-- bitonic sorter
{shuffle concat@trans@split}
{unshuffle concat@trans@pair}
{celh (lt@[1,2]->[1,2];[2,1])}
{cehl (lt@[2,1]->[1,2];[2,1])}
{done eq@[%2,length]}
{bslh (done->celh;bmlh@concat@[bslh@1,bshl@2]@split)}
{bshl (done->cehl;bmhl@concat@[bslh@1,bshl@2]@split)}
{bmlh (done->celh;concat@&bmlh@split@unshuffle@concat@&celh@pair@shuffle)}
{bmhl (done->cehl;concat@&bmhl@split@unshuffle@concat@&cehl@pair@shuffle)}
{sort bslh}
sort:<3,1,4,1,5,9,2,6>

-- quick sort
{small (null@2->id@2;(gt@[1,1@2]->apndl@[1@2,small@[1,tl@2]];small@[1,tl@2]))}
{large (null@2->id@2;(lt@[1,1@2]->apndl@[1@2,large@[1,tl@2]];large@[1,tl@2]))}
{quick (null->id;concat@[quick@small@[1,id],[1],quick@large@[1,id]]) }
quick:<3,1,4,1,5,9,2,6>
