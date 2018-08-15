package jsy.student

object Lab2 extends jsy.util.JsyApplication {
  import jsy.lab2.Parser
  import jsy.lab2.ast._

  /*
   * CSCI 3155: Lab 2
   * Sam Skolnekovich
   * 
   * Partner: Camille Owens
   * Collaborators: <Any Collaborators>
   */

  /*
   * Fill in the appropriate portions above by replacing things delimited
   * by '<'... '>'.
   * 
   * Replace 'YourIdentiKey' in the object name above with your IdentiKey.
   * 
   * Replace the '???' expression with  your code in each function.
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
   * '???' as needed to get something  that compiles without error. The ???
   * is a Scala expression that throws the exception scala.NotImplementedError.
   *
   */

  /* We represent a variable environment as a map from a string of the
   * variable name to the value to which it is bound.
   * 
   * You may use the following provided helper functions to manipulate
   * environments, which are just thin wrappers around the Map type
   * in the Scala standard library.  You can use the Scala standard
   * library directly, but these are the only interfaces that you
   * need.
   */

  type Env = Map[String, Expr]
  val emp: Env = Map()
  def get(env: Env, x: String): Expr = env(x)
  def extend(env: Env, x: String, v: Expr): Env = {
    require(isValue(v))
    env + (x -> v)
  }

  /* Some useful Scala methods for working with Scala values include:
   * - Double.NaN
   * - s.toDouble (for s: String)
   * - n.isNaN (for n: Double)
   * - n.isWhole (for n: Double)
   * - s (for n: Double)
   * - s format n (for s: String [a format string like for printf], n: Double)
   */

  def toNumber(v: Expr): Double = {
    require(isValue(v))
    (v: @unchecked) match {
      case N(n) => n
      case Undefined=>Double.NaN
      case S(s)=> try {s.toDouble} catch {case _: NumberFormatException=>Double.NaN}
      case B(false)=>0
      case B(true)=>1

    }
  }

  def toBoolean(v: Expr): Boolean = {
    require(isValue(v))
    (v: @unchecked) match {
      case B(b) => b
      case Undefined => false
      case N(n)=> n match{
        case 0=> false
        case n if(n.isNaN())=>false
        case _ =>true
      }
      case S(s)=>s != ""
      case _ => throw new UnsupportedOperationException
    }
  }

  def toStr(v: Expr): String = {
    require(isValue(v))
    (v: @unchecked) match {
      case S(s) => s
      case Undefined => "undefined"
      case N(n)=>prettyNumber(n)
      case B(b)=>b.toString()
      case _=> throw new UnsupportedOperationException
    }
  }

  def eval(env: Env, e: Expr): Expr = {
    /* Some helper functions for convenience. */
    def eToVal(e: Expr): Expr = eval(env, e)

    e match {
      // case N(n)=> eToVal(e)
      case Var(x)=>get(env,x)
      case ConstDecl(x, e1, e2) => {
        eval(extend (env, x, eToVal(e1)), e2)
      }


      case Unary(uop, e1) => {
        uop match {
          case Neg => N(-toNumber(eToVal(e1)))
          case _ => N(toNumber(eToVal(e1)))
        }
      }
      case Binary(Plus, e1, e2) => {
        (e1, e2) match {
          case ((S(_), _) | (_, S(_)))=>
              S(toStr(eToVal(e1)) + toStr(eToVal(e2)))
          case _=>
              N(toNumber(eToVal(e1)) + toNumber(eToVal(e2)))
          }
        }
      case Binary(Minus,e1,e2)=>{
        N(toNumber(eToVal(e1)) - toNumber(eToVal(e2)))
    }
      case Binary(Times,e1,e2)=>{
        N(toNumber(eToVal(e1))*(toNumber(eToVal(e2))))
    }
      case Binary(Div,e1,e2)=>{
        N(toNumber(eToVal(e1)) / toNumber(eToVal(e2)))
      }
      case Binary(Eq,e1,e2)=>{
        B(toNumber(eToVal(e1)) == toNumber(eToVal(e2)))
      }
      case Binary(Ne,e1,e2)=>{
        B(toNumber(eToVal(e1)) != toNumber(eToVal(e2)))
      }
      case Binary(Lt,e1,e2)=>{
        B(toNumber(eToVal(e1)) < toNumber(eToVal(e2)))
      }
      case Binary(Le,e1,e2)=>{
        B(toNumber(eToVal(e1)) <= toNumber(eToVal(e2)))
      }
      case Binary(Gt,e1,e2)=>{
        B(toNumber(eToVal(e1)) > toNumber(eToVal(e2)))
      }
      case Binary(Ge,e1,e2)=>{
        B(toNumber(eToVal(e1)) >= toNumber(eToVal(e2)))
      }

      case Binary(Or,e1,e2)=>{
        if(toBoolean(eToVal(e1))){
          eToVal(e1)
        }
        else
          eToVal(e2)
    }

      case Binary(And,e1,e2)=> {
        if (toBoolean(eToVal(e1)))
          eToVal(e2)
        else
          eToVal(e1)
      }
      case Binary(Seq,e1,e2) =>{
        eToVal(e1)
        eToVal(e2)
      }
      case If(e1,e2,e3)=>{
        if(toBoolean(eToVal(e1))){eToVal(e2)}
        else
          eToVal(e3)
      }
      case _ if(isValue(e))=> {
        e
      }

      /* Inductive Cases */
      case Print(e1) => println(pretty(eToVal(e1))); Undefined
      case _ => throw new UnsupportedOperationException
      }
    }

  // Interface to run your interpreter starting from an empty environment.
  def eval(e: Expr): Expr = eval(emp, e)

  // Interface to run your interpreter from a string.  This is convenient
  // for unit testing.
  def eval(s: String): Expr = eval(Parser.parse(s))

  /* Interface to run your interpreter from the command-line.  You can ignore what's below. */
  def processFile(file: java.io.File) {
    if (debug) { println("Parsing ...") }

    val expr = Parser.parseFile(file)

    if (debug) {
      println("\nExpression AST:\n  " + expr)
      println("------------------------------------------------------------")
    }

    if (debug) { println("Evaluating ...") }

    val v = eval(expr)

     println(pretty(v))
  }

}
