import jsy.util.DoWith
import org.scalatest._
import jsy.tester.JavascriptyTester
import jsy.lab5._
import jsy.lab5.ast._
import DoWith._
import jsy.lab5.Parser.parse

import jsy.student.Lab5
import Lab5._

class Lab5ExercisesSpec extends FlatSpec {

  "rename" should "rename from left-to-right using fresh" in {
    val e1 = parse("const a = 1; a")
    val e1p = parse("const x1 = 1; x1")
    val e2 = parse("const a = 2; a")
    val e2p = parse("const x2 = 2; x2")
    val e = Decl(MConst, "a", e1, e2)
    val ep = Decl(MConst, "x0", e1p, e2p)
    assertResult(ep) { rename(e) }
  }
  
  "mapFirstDoWith" should "map the first element where f returns Some" in {
     val l1 = List(1, 2, -3, 4, -5)
     val gold1 = List(1, 2, 3, 4, -5)
     def dowith[W]: DoWith[W,List[Int]] = mapFirstWith[W,Int] { (i: Int) => if (i < 0) Some(doreturn(-i)) else None } (l1)
     assertResult((true,gold1)) { dowith(true) }
     assertResult((42,gold1)) { dowith(42) }
  }

}

class Lab5InterpreterSpec extends FlatSpec {

  "CastOkNull" should "perform CastOkNull" in {
    assertResult(true) {
      castOk(TNull, TObj(Map()))
    }
  }

/*case (Function(p, Right((PName,x1,t1)), tann, e1), e2::Nil) =>
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

*/

  "DoNeg" should "return the negation of a number value" in {
    val e1 = N(5)
    val e2 = Unary(Neg, e1)
    assertResult( N(-5) ) {
      val (_, r) = step(e2)(memempty)
      r
    }
  }

  // Probably want to write some tests for castOk, typeInfer, substitute, and step.

}

// The next bit of code runs a test for each .jsy file in src/test/resources/lab4.
// The test expects a corresponding .ans file with the expected result.
class Lab5JsyTests extends JavascriptyTester(None, "lab5", Lab5)

class Lab5Suite extends Suites(
  new Lab5ExercisesSpec,
  new Lab5InterpreterSpec,
  new Lab5JsyTests
)