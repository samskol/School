package jsy.student

object Lab5 extends jsy.util.JsyApplication {
  import jsy.lab5.ast._
  import jsy.lab5._
  import jsy.util.DoWith
  import jsy.util.DoWith._

  /*
   * CSCI 3155: Lab 5
   * <Sam Skolnekovich>
   *
   * Partner: <Brooke Robinson, Dylan Cole>
   * Collaborators: <Any Collaborators>
   */

  /*
   * Fill in the appropriate portions above by replacing things delimited
   * by '<'... '>'.
   *
   * Replace the '???' expression with your code in each function.
   *
   * Do not make other modifications to this template, such as
   * - adding "extends App" or "extends Application" to your Lab object,
   * - adding a "main" method, and
   * - leaving any failing asserts.
   *
   * Your lab will not be graded if it does not compile.
   *
   * This template compiles without error. Before you submit comment out any
   * code that does not compile or causes a failing assert. Simply put in a
   * '???' as needed to get something that compiles without error. The '???'
   * is a Scala expression that throws the exception scala.NotImplementedError.
   */

  /*** Exercise with DoWith ***/

  def fresh: DoWith[Int,String] = doget flatMap { i =>
    val xp = "x" + i
    doput(i + 1) map { _ => xp }
  }

  def rename(env: Map[String, String], e: Expr): DoWith[Int,Expr] = {
    def ren(e: Expr): DoWith[Int,Expr] = rename(env, e)
    e match {
      case N(_) => doreturn(e)
      case Binary(Plus, e1, e2) =>  {
        ren(e1) flatMap { e1p =>
          ren(e2) map { e2p =>
              Binary(Plus, e1p, e2p)//increment count, could have used fresh
          }

        }
      }
      case Var(x) => doreturn(Var(env(x))) //return variable mapped to environment

	//mconst for mode.
      case Decl(MConst, x, e1, e2) => {
        //        fresh flatMap { new_name =>
        //          ren(e1) flatMap { e1p =>
        //
        //            rename(env + (x ->new_name), e2) map { e2p =>
        //              Decl(MConst, new_name, e1p, e2p)
        //            }
        //          }
        //        }
        fresh flatMap { case xp: String => ren(e1) flatMap { case e1p: Expr =>
          val envp = env + (x -> xp)
          rename(envp, e2) map { case e2p: Expr => Decl(MConst, xp, e1p, e2p) }
        }
        }
      }

      /* For this exercise, no need to implement any more cases than the ones above.
       * Leave the following default case. */
      case _ => throw new IllegalArgumentException("Gremlins: Encountered unexpected expression %s.".format(e))
    }
  }

  def rename(e: Expr): Expr = {
    val (_, r) = rename(Map.empty, e)(0)
    r
  }

  def rename(s: String): Expr = rename(Parser.parse(s))

  /*** Helper: mapFirst to DoWith ***/

  // Just like mapFirst from Lab 4 but uses a callback f that returns a DoWith in the Some case.
  def mapFirstWith[W,A](f: A => Option[DoWith[W,A]])(l: List[A]): DoWith[W,List[A]] = l match {
    case Nil => doreturn(l)
    case h :: t => f(h) match {  //unsure if this works
      case Some(x) => x map {xp => xp :: t}
      case None => mapFirstWith(f)(t) map {ft => h :: ft}
    }
  }

  /*** Casting ***/
  
  def castOk(t1: Typ, t2: Typ): Boolean = (t1, t2) match {
      /***** Make sure to replace the case _ => ???. */
    case (TNull, TObj(_)) => true
    case (TObj(fields1), TObj(fields2)) => {
      val size1 = fields1.size
      val size2 = fields2.size

      if (size1 < size2) {
        fields1 forall {
          case (key, typO) => fields2.get(key) match {
            //does everthing in fields2 exists in fields1
            case None => false
            case Some(getyp) => typO == getyp
          }
        }
      }
      else {
        fields2 forall {
          case (key, typO) => fields1.get(key) match {
            //does everthing in fields1 exists in fields2
            case None => false
            case Some(getyp) => typO == getyp
          }
        }
      }
    }
   // case (TNull, _) => true
   // case (_, TNull) => true

    case (_, _) if (t1 == t2) => true
      //case objects of same type
    //case _ => ???
      /***** Cases for the extra credit. Do not attempt until the rest of the assignment is complete. */
    case (TInterface(tvar, t1p), _) => ???
    case (_, TInterface(tvar, t2p)) => ???
      /***** Otherwise, false. */
    case _ => false
  }
  
  /*** Type Inference ***/

  type TEnv = Map[String, (Mutability,Typ)]
  val emp: TEnv = Map()
  def get(env: TEnv, x: String): (Mutability,Typ) = env(x)
  def extend(env: TEnv, x: String, mt: (Mutability,Typ)): TEnv = env + (x -> mt)

  // A helper function to check whether a jsy type has a function type in it.
  // While this is completely given, this function is worth studying to see
  // how library functions are used.
  def hasFunctionTyp(t: Typ): Boolean = t match {
    case TFunction(_, _) => true
    case TObj(fields) if (fields exists { case (_, t) => hasFunctionTyp(t) }) => true
    case _ => false
  } 

  // A helper function to translate parameter passing mode to the mutability of
  // the variable.
  def mut(m: PMode): Mutability = m match {
    case PName => MConst
    case PVar | PRef => MVar
  }
  
  def typeInfer(env: TEnv, e: Expr): Typ = {
    def typ(e1: Expr) = typeInfer(env, e1)
    def err[T](tgot: Typ, e1: Expr): T = throw StaticTypeError(tgot, e1, e)

    e match {
      case Print(e1) => typ(e1); TUndefined
      case N(_) => TNumber
      case B(_) => TBool
      case Undefined => TUndefined
      case S(_) => TString
      case Var(x) =>
        val (_, t) = env(x)
        t 			//val x return type t
      case Unary(Neg, e1) => typ(e1) match {
        case TNumber => TNumber
        case tgot => err(tgot, e1)
      }
        /***** Cases directly from Lab 4. We will minimize the test of these cases in Lab 5. */
      case Unary(Not, e1) => typ(e1) match {
        case TBool => TBool
        case tgot => err(tgot, e1)
      }
      case Binary(Plus, e1, e2) => (typ(e1), typ(e2)) match {
        case (TNumber, TNumber) => TNumber
        case (TString, TString) => TString
        case (tgot1 @ (TNumber| TString), tgot2) => err(tgot2, e2)
        case (tgot, _) => err(tgot, e1)
      }
      case Binary(Minus|Times|Div, e1, e2) => (typ(e1), typ(e2)) match {
        case (TNumber, TNumber) => TNumber
        case (TNumber, tgot2) => err(tgot2, e2)
        case (tgot, _) => err(tgot, e1)
      }
      case Binary(Eq|Ne, e1, e2) => (typ(e1), typ(e2)) match {
        case (TNumber, TNumber) => TBool
        case (TString, TString) => TBool
        case (TBool, TBool) => TBool
        case (TUndefined, TUndefined) => TBool
        case (tgot1, tgot2) if(hasFunctionTyp(tgot1)) => err(tgot1, e1)
        case (tgot1, tgot2) => err(tgot2, e2)
      }
      case Binary(Lt|Le|Gt|Ge, e1, e2) => (typ(e1), typ(e2)) match {
        case (TNumber, TNumber) => TBool
        case (TString, TString) => TBool
        case (tgot1 @ (TNumber | TString), tgot2) => err(tgot2, e2)
        case (tgot, _) => err(tgot, e1)
      }
      case Binary(And|Or, e1, e2) => (typ(e1), typ(e2)) match {
        case (TBool, TBool) => TBool
        case (_, tgot) => err(tgot, e2)
      }
      case Binary(Seq, e1, e2) => {
        typ(e1)
        typ(e2)
      }
      case If(e1, e2, e3) => (typ(e1), typ(e2), typ(e3)) match {
        case (TBool, t2, t3) => if(t2 == t3) t2 else err(t3, e3)
        case (tgot1, _, _) => err(tgot1, e1)
      }

	//fields.mapValues is 
      case Obj(fields) => TObj(fields.mapValues(e => typ(e))) //map expression to 
								//expression type
      case GetField(e1, f) =>  typ(e1) match {
        case TObj(fields) => fields.get(f) match {
          case Some(t) => t     // case some type matches
          case None => err(typ(e1), e1)
        }
        case _ => err(typ(e1), e1)
      }


        /***** Cases from Lab 4 that need a small amount of adapting. */
      case Decl(mut, x, e1, e2) => typeInfer(extend(env, x, (mut, typ(e1))), e2)
      case Function(p, params, tann, e1) => {
        // Bind to env1 an environment that extends env with an appropriate binding if
        // the function is potentially recursive.
        val env1 = (p, tann) match {
          case (Some(f), Some(tret)) =>
            val tprime = TFunction(params, tret)
            extend(env, f, (MConst, tprime)) //
          case (None, _) => env
          case _ => err(TUndefined, e1)
        }
        // Bind to env2 an environment that extends env1 with bindings for params.
        val env2 = params match {
          case Left(paramsList) => paramsList.foldLeft(env1) {
            case (acc, (p_name, p_type)) => extend(acc, p_name, (MConst, p_type))
          }
          case Right((mode, x, tau)) => mode match {
            case PName => extend(env1, x, (MConst, tau))
            case _ => extend(env1, x, (MVar, tau))
        }
        }
        // Match on whether the return type is specified.
        val type1 = typeInfer(env2, e1)
        tann match {
          case None => TFunction(params, typeInfer(env2, e1))
          case Some(tret) => {
            if (TFunction(params, tret) != TFunction(params, type1)) err(TFunction(params, type1), e1)
            else TFunction(params, type1)
          }
        }
      }
      case Call(e1, args) => typ(e1) match {
        case TFunction(Left(params), tret) if (params.length == args.length) =>
          (params, args).zipped.foreach {
            case ((p_name, p_type), arg) => {
              if (p_type != typ(arg)) err(typ(arg), arg)
              else typ(arg)
            }
          };
          tret
        case tgot @ TFunction(Right((mode,_,tparam)), tret) if (args.length == 1) =>  mode match {
		//name and var occur in function call.
          case PName | PVar => {
            val headtyp = typ(args.head)
            if (headtyp != tparam) err(headtyp, args.head)
            else tret
          }
		// ref occurs before function call using that val and then overwriting,basically
          case PRef if isLExpr(args.head) => {
            val headtyp = typ(args.head)
            if (headtyp != tparam) err(headtyp, args.head)
            else tret
          }
          case _ => err(typ(args.head), args.head)
        }

        case tgot => err(tgot, e1)
      }

        /***** New cases for Lab 5. ***/
      case Assign(Var(x), e1) => {
        env(x) match {
          case (MVar, typvar) if typvar == typ(e1) => typvar
          case _ => err(typ(e1), e1)
        }
      }
      case Assign(GetField(e1, f), e2) => typ(e1) match {
        case TObj(fields) => {
          val ftype = fields(f)
          if(ftype == typ(e2)) typ(e2)
          else err(typ(e2), e2)
        }
        case tgot => err(typ(e1), e1)
      }
      case Assign(_, _) => err(TUndefined, e)

      case Null => TNull

      case Unary(Cast(t), e1) => typ(e1) match {
        case tgot if castOk(tgot, t) => t
        case tgot => err(tgot, e1)
      }

      /* Should not match: non-source expressions or should have been removed */
      case A(_) | Unary(Deref, _) | InterfaceDecl(_, _, _) => throw new IllegalArgumentException("Gremlins: Encountered unexpected expression %s.".format(e))
    }
  }
  
  /*** Small-Step Interpreter ***/

  /*
   * Helper function that implements the semantics of inequality
   * operators Lt, Le, Gt, and Ge on values.
   *
   * We suggest a refactoring of code from Lab 2 to be able to
   * use this helper function in eval and step.
   *
   * This should the same code as from Lab 3 and Lab 4.
   */
  def inequalityVal(bop: Bop, v1: Expr, v2: Expr): Boolean = {
    require(isValue(v1), "v1 in inequalityVal is not a value")
    require(isValue(v2), "v2 in inqualityVal is not a value")
    require(bop == Lt || bop == Le || bop == Gt || bop == Ge)
    ((v1, v2): @unchecked) match {
      case (S(s1), S(s2)) => (bop: @unchecked) match {
        case Lt => s1 < s2
        case Le => s1 <= s2
        case Gt => s1 > s2
        case Ge => s1 >= s2
      }
      case (N(n1), N(n2)) => (bop: @unchecked) match {
        case Lt => n1 < n2
        case Le => n1 <= n2
        case Gt => n1 > n2
        case Ge => n2 >= n2
      }
    }
  }
  
  /* Capture-avoiding substitution in e replacing variables x with esub. */
  def substitute(e: Expr, esub: Expr, x: String): Expr = {
    /* We removed the requirement that esub is a value to support call-by-name. */
    //require(isValue(esub), "Expr to substitute is not a value")
    /* We suggest that you add support for call-by-name last. */
    def subst(e: Expr): Expr = substitute(e, esub, x)
    val ep: Expr = avoidCapture(freeVars(esub), e)    /**Does this work??? **/
    ep match {
      case N(_) | B(_) | Undefined | S(_) | Null | A(_) => e
      case Print(e1) => Print(subst(e1))
        /***** Cases from Lab 3 */
      case Unary(uop, e1) =>  Unary(uop, subst(e1))
      case Binary(bop, e1, e2) => Binary(bop, subst(e1), subst(e2))
      case If(e1, e2, e3) => If(subst(e1), subst(e2), subst(e3))
      case Var(y) =>  if (y == x) esub else ep     //CHECK TO MAKE SURE THIS WORKS
        /***** Cases need a small adaption from Lab 3 */
      case Decl(mut, y, e1, e2) => Decl(mut, y, subst(e1), if (x == y) e2 else subst(e2))
        /***** Cases needing adapting from Lab 4 */
      case Function(p, paramse, retty, e1) => {
        //if func has name or its has parameters, check to match with x ad ret function
        if(p == Some(x) || paramse.fold(params => params exists
        {case (y, _) => y == x}, {case (_, y, _) => y == x})) e
        else Function(p, paramse, retty, subst(e1))
      }
        /***** Cases directly from Lab 4 */
      case Call(e1, args) => Call(subst(e1), args map subst)
      case Obj(fields) => Obj(fields mapValues subst)
      case GetField(e1, f) => GetField(subst(e1), f)
        /***** New case for Lab 5 */
      case Assign(e1, e2) => Assign(subst(e1), subst(e2))
        /***** Extra credit case for Lab 5 */
      case InterfaceDecl(tvar, t, e1) => ???
    }
  }

  /* A small-step transition. */
  def step(e: Expr): DoWith[Mem, Expr] = {
    require(!isValue(e), "stepping on a value: %s".format(e))
    
    /*** Helpers for Call ***/
    
    def stepIfNotValue(e: Expr): Option[DoWith[Mem,Expr]] = if (isValue(e)) None else Some(step(e))
    
    /* Check whether or not the argument expression is ready to be applied. */
    def argApplyable(mode: PMode, arg: Expr): Boolean = mode match {
      case PVar => isValue(arg)
      case PName => true
      case PRef => isLValue(arg)
    }

    /*** Body ***/
    e match {
      /* Base Cases: Do Rules */
      case Print(v1) if isValue(v1) => doget map { m => println(pretty(m, v1)); Undefined }
        /***** Cases needing adapting from Lab 3. Make sure to replace the case _ => ???. */
      case Unary(Neg, N(n1))=> doreturn(N(-n1))
      case Unary(Not, B(b1)) => doreturn(B(!b1))
      case Binary(Seq, v1, e2) if isValue(v1) => doreturn(e2)

      case Binary(Plus, N(n1), N(n2)) => doreturn(N(n1 + n2))
      case Binary(Plus, S(s1), S(s2)) => doreturn(S(s1 + s2))
      case Binary(Minus, N(n1), N(n2)) => doreturn(N(n1 - n2))
      case Binary(Times, N(n1), N(n2)) => doreturn(N(n1 * n2))
      case Binary(Div, N(n1), N(n2)) => doreturn(N(n1 / n2))

      case Binary(bop @ (Lt|Le|Gt|Ge), v1, v2 ) if isValue(v1) && isValue(v2) => doreturn(B(inequalityVal(bop, v1, v2)))

      case Binary(Eq, v1, v2) if isValue(v1) && isValue(v2) => doreturn(B(v1 == v2))
      case Binary(Ne, v1, v2) if isValue(v1) && isValue(v2) => doreturn(B(v1 != v2))

      case Binary(And, B(bool1), e2) => doreturn {if (bool1) e2 else B(false)}
      case Binary(Or, B(bool1), e2) => doreturn {if (bool1) B(true) else e2}


      case If(B(b1), e2, e3) => doreturn {if (b1) e2 else e3}
        /***** Cases needing adapting from Lab 4. Make sure to replace the case _ => ???. */
      case Obj(fields) if (fields forall { case (_, vi) => isValue(vi)}) =>
        memalloc(e)
      case GetField(a @ A(_), f) =>
        doget.map {
          (m: Mem) => m.get(a) match {
            case Some(Obj(fields)) => fields.get(f) match {
              case Some(value) => value
              case None => throw StuckError(e)
          }
            case _ => throw StuckError(e)
          }
        }
        ///my stuff
      case Call(v1, args) if isValue(v1) =>
        def substfun(e1: Expr, p: Option[String]): Expr = p match {
          case None => e1
          case Some(x) => substitute(e1, v1, x)
        }
        (v1, args) match {
          /** * Fill-in the DoCall cases, the SearchCall2, the SearchCallVar, the SearchCallRef  ***/
          //DoCall and DoCallRec

          case (Function(p,Left(params),tann,e1),args) if (args forall isValue) =>
            if (params.length != args.length) throw StuckError(e);
            val e1p = (params,args).zipped.foldRight(e1) { case (((p,_),a),acc) => substitute(acc,a,p) }
            doreturn(substfun(e1p, p))

          /* SearchCall2 */
          case (Function(_, Left(_),_,_), args) => mapFirstWith(stepIfNotValue)(args) map {argsp => Call(v1, argsp)}

          //DoCallName and DoCallRecName
          // argument passed to func == expr, and not fully evaluated. it must be in a List form because args is a List of expr
          //matching on tuple function "type" and args "list"
          //p = name; Right = use param modes;
          case (Function(p, Right((PName,x1,t1)), tann, e1), e2::Nil) =>
            //return a DoWith with result of substfun, p == optional name. If function has no name, ret the evaluation of the expr
            //substitute(e1,e2,x1) (eval e1 given e2 replaces all instances of x1)
            //If function has name, all instances of that name in e2 are replaced by the name by substfun inside the expr substitute(e1,e2,x1)
            doreturn(substfun(substitute(e1,e2,x1),p))

          //DoCallVar and DoCallRecVar
          //Must nesure v2 is a vlue
          case (Function(p, Right((PVar,x1,t1)), tann, e1), v2::Nil) if (isValue(v2)) =>
            //allocate new mem, extends the memory for us with value v2, and rets a DoWith with the addr, a.
            //map to put in our return DoWith, where all instances of x1 in e1 replaced with the deref of a.
            //Wrap the subst in substfun with the optional name p, if p is a Some type, the function name will also be subst into the expr.
            memalloc(v2) map { a => substfun(substitute(e1, Unary(Deref,a), x1),p)}

          //DoCallRef and DoCallRecRef
          //lv2 must be a location value
          case (Function(p, Right((PRef, x1, t1)), tann, e1), lv2 :: Nil) if (isLValue(lv2)) =>
            doreturn(substfun(substitute(e1,lv2,x1),p))

          /* SearchCallVar */
          case (Function(p, Right((PVar, _, _)), _, e1),e2::Nil) => step(e2) map {e2p => Call(v1, e2p::Nil)}

          /* SearchCallRef */
          case (Function(p, Right((PRef, _, _)), _, e1), e2::Nil) if (!isLValue(e2)) => step(e2) map {e2p => Call(v1, e2p::Nil)}

          case _ => throw StuckError(e)
        }
      
      case Decl(MConst, x, v1, e2) if isValue(v1) => doreturn(substitute(e2, v1, x))
      case Decl(MVar, x, v1, e2) if isValue(v1) => memalloc(v1) flatMap {addr =>  //get new addr
        domodify {memory: Mem => memory + (addr -> v1)} map { _ => //update mem to have addr maped to val
          substitute(e2, Unary(Deref, addr), x)} } //transform result

        /***** New cases for Lab 5. */
      case Unary(Deref, a @ A(_)) => doget map
        {memory => memory.get(a) match {
          case Some(x) => x
          case None => throw StuckError(e)
        }}

      case Assign(Unary(Deref, a @ A(_)), v) if isValue(v) =>
        domodify[Mem] { m => m + (a->v) } map { _ => v }

      case Assign(GetField(a @ A(_), f), v) if isValue(v) =>
        domodify[Mem] {m => m.get(a) match {
          case Some(Obj(fields)) if (fields.contains(f)) => m + (a, Obj(fields + (f -> v)))
          case Some(Null) => throw NullDereferenceError(e)
          case _ => throw StuckError(e)
        }} map {_ => v}

      /* Base Cases: Error Rules */
        /***** Replace the following case with a case to throw NullDeferenceError.  */

      //DoCastNULL
      case Unary(Cast(TObj(_)), Null) => doreturn(Null)

        //DoCastObj
      case Unary(Cast(TObj(fields)), a @ A(_)) => doget map { memory => memory.get(a) match {
        //case Obj(fields) if (fields forall { case (_, vi) => isValue(vi)}) =>memalloc(e)
        case Some(Obj(fieldsC)) if fields forall { case (fi, _) => fieldsC.contains(fi)} => a
        case _ => throw DynamicTypeError(e)
      }
      }
        //DoCast
      case Unary(Cast(t), v) if isValue(v) => v match {
        case Null | A(_) => throw StuckError(e) //do Something
        case _ => doreturn(v)
      }

        //NullErrorGetField
      case GetField(Null, f) => throw NullDereferenceError(e)

      case Assign(GetField(Null, f), _) => throw NullDereferenceError(e)


      //case _ => throw NullDeferenceError(e)

      /* Inductive Cases: Search Rules */
        /***** Cases needing adapting from Lab 3. Make sure to replace the case _ => ???. */
      case Print(e1) => step(e1) map { e1p => Print(e1p) }
      case Unary(uop, e1) => step(e1) map {e1p => Unary(uop, e1p)}
      case Binary(bop, v1, e2) if isValue(v1) => step(e2) map {e2p => Binary(bop, v1, e2p)}
      case Binary(bop, e1, e2) => step(e1) map {e1p => Binary(bop, e1p, e2)}
      case If(e1, e2, e3) => step(e1) map {e1p => If(e1p, e2, e3) }
      //SearchCall1
      case Call(e1, args) => step(e1) map {e1p => Call(e1p, args)}
      case Decl(mut, x, e1, e2) => step(e1) map { e1p => Decl(mut, x, e1p, e2)}
        /***** Cases needing adapting from Lab 4 */
      case GetField(e1, f) => step(e1) map {e1p => GetField(e1p, f)}
      case Obj(fields) =>
        val toStep = fields.find (
        { case (fieldName, fieldExpr) => !isValue(fieldExpr)})
        toStep match
          {
          case Some((field, expr)) => step(expr) map {exprPrime => Obj(fields + (field -> exprPrime))}
          case None => throw StuckError(e)
        }

        /***** New cases for Lab 5.  */
      case Assign(e1, e2) if isLValue(e1) => step(e2) map {e2p => Assign(e1, e2p)}
      case Assign(e1, e2) => step(e1) map {e1p => Assign(e1p, e2)}

      /* Everything else is a stuck error. */
      case _ => throw StuckError(e)
    }
  }

  /*** Extra Credit: Lowering: Remove Interface Declarations ***/

  def removeInterfaceDecl(e: Expr): Expr =
    /* Do nothing by default. Change to attempt extra credit. */
    e

  /*** External Interfaces ***/

  //this.debug = true // comment this out or set to false if you don't want print debugging information
  this.maxSteps = Some(1000) // comment this out or set to None to not bound the number of steps.
  
  def inferType(e: Expr): Typ = {
    if (debug) {
      println("------------------------------------------------------------")
      println("Type checking: %s ...".format(e))
    } 
    val t = typeInfer(Map.empty, e)
    if (debug) {
      println("Type: " + pretty(t))
    }
    t
  }
  
  // Interface to run your small-step interpreter and print out the steps of evaluation if debugging. 
  
  case class TerminationError(e: Expr) extends Exception {
    override def toString = Parser.formatErrorMessage(e.pos, "TerminationError", "run out of steps in evaluating " + e)
  }
  
  def iterateStep(e: Expr): Expr = {
    require(closed(e), "input Expr to iterateStep is not closed: free variables: %s".format(freeVars(e)) )
    def loop(e: Expr, n: Int): DoWith[Mem,Expr] =
      if (Some(n) == maxSteps) throw TerminationError(e)
      else if (isValue(e)) doreturn( e )
      else {
        for {
          m <- doget[Mem]
          _ = if (debug) { println("Step %s:%n  %s%n  %s".format(n, m, e)) }
          ep <- step(e)
          epp <- loop(ep, n + 1)
        } yield
        epp
      }
    if (debug) {
      println("------------------------------------------------------------")
      println("Evaluating with step ...")
    }
    val (m,v) = loop(e, 0)(memempty)
    if (debug) {
      println("Result:%n  %s%n  %s".format(m,v))
    }
    v
  }

  // Convenience to pass in a jsy expression as a string.
  def iterateStep(s: String): Expr = iterateStep(removeInterfaceDecl(jsy.lab5.Parser.parse(s)))

  this.keepGoing = true // comment this out if you want to stop at first exception when processing a file

  // Interface for main
  def processFile(file: java.io.File) {
    if (debug) {
      println("============================================================")
      println("File: " + file.getName)
      println("Parsing ...")
    }
    
    val expr =
      handle(None: Option[Expr]) {Some{
        jsy.lab5.Parser.parseFile(file)
      }} getOrElse {
        return
      }
      
    val exprlowered =
      handle(None: Option[Expr]) {Some{
        removeInterfaceDecl(expr)
      }} getOrElse {
        return
      }

    val welltyped = handle(false) {
      println("# Type checking ...")
      val t = inferType(exprlowered)
      println("## " + pretty(t))
      true
    }
    if (!welltyped) return

    handle() {
      println("# Stepping ...")
      def loop(e: Expr, n: Int): DoWith[Mem,Expr] =
        if (Some(n) == maxSteps) throw TerminationError(e)
        else if (isValue(e)) doreturn( e )
        else {
          for {
            m <- doget[Mem]
            _ = println("## %4d:%n##  %s%n##  %s".format(n, m, e))
            ep <- step(e)
            epp <- loop(ep, n + 1)
          } yield
          epp
        }
      val (m,v) = loop(exprlowered, 0)(memempty)
      println("## %s%n%s".format(m,pretty(v)))
    }
  }
    
}