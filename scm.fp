
-- a subset-of-scheme interpreter written in FP
	
-- lookup: <ide,env> -> val or bottom
{lookup	(null@2 -> %wrong ; (eq@[1,1@2] -> 2@2 ; lookup@[1,3@2]))}

-- doprim: <ide,val> -> val
{doprim (eq@[id,%succ]@1 -> +@[id,%1]@2;
	(eq@[id,%pred]@1 -> -@[id,%1]@2;
	(eq@[id,%zerop]@1 -> eq@[id,%0]@2;
	%wrong)))}

-- eval: <exp,env> -> val
{eval	(atom@1 -> lookup@[1,2];
	(eq@[id,%if]@1@1 -> eval@[(eval@[2@1,2]->3@1;4@1),2];
	(eq@[id,%lam]@1@1 -> [%clo,2@1,3@1,2];
	(eq@[id,%app]@1@1 -> apply@[eval@[2@1,2],eval@[3@1,2]];
	%wrong))))}

-- apply: <fun,arg> -> val
{apply	(eq@[id,%prim]@1@1 -> doprim@[2@1,2];
	(eq@[id,%clo]@1@1 -> eval@[3@1,[2@1,2,4@1]];
	%wrong))}

-- init: <> -> env
{init	%<succ,<prim,succ>,
	 <pred,<prim,pred>,
	 <zerop,<prim,zerop>,
	 <five,5,
	 <three,3,
	 <Z,<clo,f,<app,<lam,x,<app,f,<lam,z,<app,<app,x,x>,z>>>>,
			<lam,x,<app,f,<lam,z,<app,<app,x,x>,z>>>>>,<>>,
	 wrong>>>>>>}

-- foo, sets up an inititial env with plus bound
{pluse	%<app,Z,<lam,p,<lam,x,<lam,y,<if,<app,zerop,x>,
					 y,
					 <app,<app,p,<app,pred,x>>,
					      <app,succ,y>>>>>>>}
{foo	[%app,[%lam,%plus,id],pluse]}

-- xxx, like eval but inits the env first
-- (foo wraps the def for plus around the expr)
{xxx	eval@[foo,init]}

-- 3+5 ==> 8
xxx:<app,<app,plus,three>,five>

